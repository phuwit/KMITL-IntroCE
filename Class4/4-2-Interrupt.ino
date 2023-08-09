const int BUTTON_COUNT = 3;
const int BUTTON[BUTTON_COUNT] = { 3, 4, 2 };
const char BUTTON_TYPE[BUTTON_COUNT] = { 'D', 'U', 'I' };
int buttonState[BUTTON_COUNT];
int lastButtonState[BUTTON_COUNT] = { LOW };
unsigned long lastDebounceTime[BUTTON_COUNT] = { 0 };  // the last time the output pin was toggled
const unsigned long debounceDelay = 50;                // the debounce time; increase if the output flickers

const int LED_COUNT = 3;
const int LED[LED_COUNT] = { 12, 11, 10 };
int ledStatus[LED_COUNT] = { LOW, HIGH, HIGH };

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
  attachInterrupt(1, func1, RISING);
  attachInterrupt(0, func2, FALLING);
}

void loop() {
  digitalWrite(LED[2], HIGH);
  delay(7000);
  digitalWrite(LED[2], LOW);
  for (int i = 0; i < 6; i++) {
    digitalWrite(LED[1], HIGH);
    delay(250);
    digitalWrite(LED[1], LOW);
    delay(250);
  }
  digitalWrite(LED[0], HIGH);
  delay(5000);
  digitalWrite(LED[0], LOW);
}

void func1() {
  int ledStatus[3];
  for (int i = 0; i < 3; i++) {
    ledStatus[i] = digitalRead(LED[i]);
  }
  digitalWrite(LED[1], LOW);
  digitalWrite(LED[2], LOW);
  digitalWrite(LED[0], HIGH);
  for (int i = 0; i < 1000; i++) {
    delayMicroseconds(5000);
  }
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED[i], ledStatus[i]);
  }
}

void func2() {
  int ledStatus[3];
  for (int i = 0; i < 3; i++) {
    ledStatus[i] = digitalRead(LED[i]);
  }
  for (int i = 0; i < LED_COUNT; i++) {
    digitalWrite(LED[i], LOW);
  }
  for (int i = 0; i < 1000; i++) {
    delayMicroseconds(5000);
  }
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED[i], ledStatus[i]);
  }
}
