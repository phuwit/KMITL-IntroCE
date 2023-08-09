const int BUTTON_COUNT = 3;
const int BUTTON[BUTTON_COUNT] = {3, 4, 2};
int buttonState[BUTTON_COUNT];
int lastButtonState[BUTTON_COUNT] = {LOW};
unsigned long lastDebounceTime[BUTTON_COUNT] = {0};  // the last time the output pin was toggled
const unsigned long debounceDelay = 50;  // the debounce time; increase if the output flickers

const int LED_COUNT = 3;
const int LED[LED_COUNT] = {12, 11, 10};
int ledStatus[LED_COUNT] = {LOW, HIGH, HIGH};

void setup() {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        pinMode(BUTTON[i], INPUT);
    }

    pinMode(BUTTON[2], INPUT_PULLUP);

    for (int i = 0; i < LED_COUNT; i++) {
        pinMode(LED[i], OUTPUT);
    }
}

void loop() {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        deBounce(i);

        // Active
        ledStatus[i] = buttonState[i];

        // // Toggle
        // if (buttonState[i] == HIGH) {
        //     ledStatus[i] = !ledStatus[i];
        // }

        digitalWrite(LED[i], ledStatus[i]);
    }
}

void deBounce(int buttonIndex) {
    // read the state of the switch into a local variable:
    int reading = digitalRead(BUTTON[buttonIndex]);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState[buttonIndex]) {
        // reset the debouncing timer
        lastDebounceTime[buttonIndex] = millis();
    }

    if ((millis() - lastDebounceTime[buttonIndex]) > debounceDelay) {
        // whatever the reading is at, it's been there for longer than the
        // debounce delay, so take it as the actual current state:

        // if the button state has changed:
        if (reading != buttonState[buttonIndex]) {
            buttonState[buttonIndex] = reading;
        }
    }

    // save the reading. Next time through the loop, it'll be the
    // lastButtonState:
    lastButtonState[buttonIndex] = reading;
}