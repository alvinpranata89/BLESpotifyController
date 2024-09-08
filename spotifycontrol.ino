#include <BleKeyboard.h>

//gpio number! not digital pin number!
const int buttonPlayPause = 5;
const int buttonNext = 21;
const int buttonPrev = 7;
const int buttonVolUp = 3;
const int buttonVolDown = 20;

// Debounce parameters
unsigned long lastDebounceTime[5] = {0, 0, 0, 0, 0};
unsigned long debounceDelay = 30;

// Previous button states
int lastButtonState[5] = {LOW, LOW, LOW, LOW, LOW};

BleKeyboard bleKeyboard;

void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE work!");
    bleKeyboard.begin();
    
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPlayPause, INPUT_PULLDOWN);
    pinMode(buttonNext, INPUT_PULLDOWN);
    pinMode(buttonPrev, INPUT_PULLDOWN);
    pinMode(buttonVolUp, INPUT_PULLDOWN);
    pinMode(buttonVolDown, INPUT_PULLDOWN);
}

void handleButton(int buttonPin, int index, const char* action, const uint8_t* key) {
    int reading = digitalRead(buttonPin);
    
    // Check for button state change and debounce
    if (reading != lastButtonState[index]) {
        lastDebounceTime[index] = millis();
        //  Serial.println(*lastDebounceTime);
        //  int a= millis() - lastDebounceTime[index];
        //  Serial.printf("a: %d", a);
    }

    if ((millis() - lastDebounceTime[index]) > debounceDelay) {
     
        if (reading == HIGH && lastButtonState[index] == HIGH) {
            Serial.printf("Sending %s media key...\n", action);
            bleKeyboard.write(key);
            ledState = !ledState;
            digitalWrite(ledPin, ledState);
            delay(100); // Delay for stability
            bleKeyboard.releaseAll();
        }
    }

    lastButtonState[index] = reading;
}

void loop() {
    if (bleKeyboard.isConnected()) {
        handleButton(buttonPlayPause, 0, "Play/Pause", KEY_MEDIA_PLAY_PAUSE);
        handleButton(buttonNext, 1, "Next", KEY_MEDIA_NEXT_TRACK);
        handleButton(buttonPrev, 2, "Previous", KEY_MEDIA_PREVIOUS_TRACK);
        handleButton(buttonVolUp, 3, "Volume Up", KEY_MEDIA_VOLUME_UP);
        handleButton(buttonVolDown, 4, "Volume Down", KEY_MEDIA_VOLUME_DOWN);
    }

    vTaskDelay(50 / portTICK_PERIOD_MS); // Yielding CPU to prevent watchdog
}
