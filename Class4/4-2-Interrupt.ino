const int BUTTON_COUNT = 3;
const int BUTTON[BUTTON_COUNT] = {3, 4, 2};
const char BUTTON_TYPE[BUTTON_COUNT] = {'D', 'U', 'I'};
int buttonState[BUTTON_COUNT];
int lastButtonState[BUTTON_COUNT] = {LOW};
unsigned long lastDebounceTime[BUTTON_COUNT] = {0};  // the last time the output pin was toggled
const unsigned long debounceDelay = 50;  // the debounce time; increase if the output flickers

const int LED_COUNT = 3;
const int LED[LED_COUNT] = {12, 11, 10};
int ledStatus[LED_COUNT] = {LOW, HIGH, HIGH};

void setup() {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        if (BUTTON_TYPE[i] == 'I') {
            pinMode(BUTTON[i], INPUT_PULLUP);
        }
        else {
            pinMode(BUTTON[i], INPUT);
        }
    }

    for (int i = 0; i < LED_COUNT; i++) {
        pinMode(LED[i], OUTPUT);
    }
    attachInterrupt(BUTTON[0],func1, CHANGE);
    attachInterrupt(BUTTON[2], func2, CHANGE);
}

void loop() {
  digitalWrite(LED[2], HIGH);
  delay(7000);
  digitalWrite(LED[2], LOW);
  for (int i  = 0; i < 6000 ; i++)
  {
    digitalWrite(LED[1], HIGH);
    delay(0.25);
    digitalWrite(LED[1], LOW);
    delay(0.25);
  }
  digitalWrite(LED[0], HIGH)
}

void func1()
{
  digitalWrite(LED[1], LOW);
  digitalwrite(LED[2], LOW);
  digitalWrite(LED[0], HIGH);
  delay(5000);
}

void func2()
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    digitalWrite(LED[i], LOW);
  }
  delay(5000)
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

    if (BUTTON_TYPE[buttonIndex] != 'D') {
        buttonState[buttonIndex] = !buttonState[buttonIndex];
    }
}