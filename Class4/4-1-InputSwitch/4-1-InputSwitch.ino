const int BUTTON_COUNT = 3;
const int BUTTON[BUTTON_COUNT] = { 3, 4, 2 };
const char BUTTON_TYPE[BUTTON_COUNT] = { 'D', 'U', 'I' };
int buttonState[BUTTON_COUNT];
int lastButtonState[BUTTON_COUNT] = { LOW };
unsigned long lastDebounceTime[BUTTON_COUNT] = { 0 };  // the last time the output pin was toggled
const unsigned long debounceDelay = 50;                // the debounce time; increase if the output flickers

const int LED_COUNT = 3;
const int LED[LED_COUNT] = { 12, 11, 10 };
int ledStatus[LED_COUNT] = { LOW };

void setup() {
  for (int i = 0; i < BUTTON_COUNT; i++) {
    if (BUTTON_TYPE[i] == 'I') {
      pinMode(BUTTON[i], INPUT_PULLUP);
    } else {
      pinMode(BUTTON[i], INPUT);
    }
  }

  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(LED[i], OUTPUT);
  }

  Serial.begin(9600);
}

void loop() {
  for (int buttonIndex = 0; buttonIndex < BUTTON_COUNT; buttonIndex++) {
    // read the state of the switch into a local variable:
    int reading = digitalRead(BUTTON[buttonIndex]);

    if (BUTTON_TYPE[buttonIndex] != 'D') {
      reading = !reading;
    }

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState[buttonIndex]) {
      // reset the debouncing timer
      lastDebounceTime[buttonIndex] = millis();
    }

    if ((millis() - lastDebounceTime[buttonIndex]) > debounceDelay) {
      // whatever the reading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state:

      // if the button state has changed:
      if (reading != buttonState[buttonIndex]) {
        buttonState[buttonIndex] = reading;

        // only toggle the LED if the new button state is HIGH
        if (buttonState[buttonIndex] == HIGH) {
          ledStatus[buttonIndex] = !ledStatus[buttonIndex];
        }
      }
    }

    // save the reading. Next time through the loop, it'll be the
    // lastButtonState:
    lastButtonState[buttonIndex] = reading;

    digitalWrite(LED[buttonIndex], ledStatus[buttonIndex]);
  }
  Serial.print(buttonState[1]);
  Serial.println(ledStatus[1]);
}