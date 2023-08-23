#include "pitches.h"

const uint8_t SPEAKER_PIN = 3;

const uint8_t BUTTON_COUNT = 6;
const uint8_t BUTTON_PIN[BUTTON_COUNT] = {12, 11, 10, 9, 8, 7};
const uint8_t PIANO_PIN_FREQUENCY[BUTTON_COUNT - 2] = {NOTE_E3, NOTE_F3, NOTE_G3, NOTE_A3};

// deBounce stuffs
uint8_t buttonState[BUTTON_COUNT] = {LOW};
uint8_t lastButtonState[BUTTON_COUNT] = {LOW};
uint8_t readableButtonState[BUTTON_COUNT];
uint8_t lastReadableButtonState[BUTTON_COUNT] = {LOW};

unsigned long lastDebounceTime[BUTTON_COUNT] = {0};
const unsigned long debounceDelay = 50;

uint8_t deBounce(const uint8_t &buttonPin, uint8_t &buttonState,
                 uint8_t &lastButtonState, unsigned long &lastDebounceTime);

int playHistory[100] = {0};
int currentHistoryIndex = 0;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    pinMode(SPEAKER_PIN, OUTPUT);
    for (int i = 0; i < BUTTON_COUNT; i++) {
        pinMode(BUTTON_PIN[i], INPUT_PULLUP);
    }
}

void loop() {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        // debounce all buttons
        readableButtonState[i] = deBounce(BUTTON_PIN[i], buttonState[i], lastButtonState[i], lastDebounceTime[i]);
        // readableButtonState[i] = !digitalRead(BUTTON_PIN[i]);
        // Serial.println(readableButtonState[0]);

        // check if replay
        if (i == 5 && readableButtonState[i] == HIGH) {
            Serial.println("replay history");
            Serial.println(currentHistoryIndex);
            for (int i = 0; i < currentHistoryIndex; i++) {
                tone(SPEAKER_PIN, PIANO_PIN_FREQUENCY[i]);
                delay(500);
              noTone(SPEAKER_PIN);
              delay(50);
            }
        }

        // check if reset
        if (i == 4 && readableButtonState[i] == HIGH) {
            Serial.println("reset history");
            currentHistoryIndex = 0;
        }

        if (readableButtonState[i] == HIGH && i <= 3) {
            Serial.print("played pin");
            Serial.println(i);
            // play tone
            tone(SPEAKER_PIN, PIANO_PIN_FREQUENCY[i]);
            // save history
            playHistory[currentHistoryIndex] = PIANO_PIN_FREQUENCY[i];
            currentHistoryIndex++;

            delay(500);
            noTone(SPEAKER_PIN);
        }
    }
}

uint8_t deBounce(const uint8_t &buttonPin, uint8_t &buttonState,
                 uint8_t &lastButtonState, unsigned long &lastDebounceTime) {
    int readableButtonState = LOW;

    // read the state of the switch into a local variable:
    int reading = digitalRead(buttonPin);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState) {
        // reset the debouncing timer
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        // whatever the reading is at, it's been there for longer than the
        // debounce delay, so take it as the actual current state:

        // if the button state has changed:
        if (reading != buttonState) {
            buttonState = reading;

            // only toggle the LED if the new button state is HIGH
            if (buttonState == HIGH) {
                readableButtonState = HIGH;
            }
        }
    }

    // save the reading. Next time through the loop, it'll be the
    // lastButtonState:
    lastButtonState = reading;

    return readableButtonState;
}
