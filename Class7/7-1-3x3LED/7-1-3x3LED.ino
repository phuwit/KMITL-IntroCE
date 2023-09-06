const uint8_t LED_PIN_X[3] = {12, 11, 10};
const uint8_t LED_PIN_Y[3] = {7, 6, 5};

unsigned uint8_t SCAN_TIME = 10;

void drawX(int timeMs);
void drawO(int timeMs);

void setup() {
    for (int i = 0; i < 3; i++) {
        pinMode(LED_PIN_X[i], OUTPUT);
        pinMode(LED_PIN_Y[i], OUTPUT);
    }
}

void loop() {
    drawX(2000);
}

void drawX(int timeMs) {
    unsigned long endTime = millis() + timeMs;
    while (millis() < endTime) {
        digitalWrite(LED_PIN_X[0], HIGH);
        digitalWrite(LED_PIN_Y[0], HIGH);
        delay(SCAN_TIME);
        digitalWrite(LED_PIN_Y[0], LOW);
        digitalWrite(LED_PIN_Y[2], HIGH);
        delay(SCAN_TIME);
        digitalWrite(LED_PIN_X[0], LOW);
        digitalWrite(LED_PIN_Y[2], LOW);
        digitalWrite(LED_PIN_X[1], HIGH);
        digitalWrite(LED_PIN_Y[1], HIGH);
        delay(SCAN_TIME);
        digitalWrite(LED_PIN_X[1], LOW);
        digitalWrite(LED_PIN_Y[1], LOW);
        digitalWrite(LED_PIN_X[2], HIGH);
        digitalWrite(LED_PIN_Y[0], HIGH);
        delay(SCAN_TIME);
        digitalWrite(LED_PIN_Y[0], LOW);
        digitalWrite(LED_PIN_Y[2], HIGH);
        delay(SCAN_TIME);
        digitalWrite(LED_PIN_X[2], LOW);
        digitalWrite(LED_PIN_Y[2], LOW);
    }
}

void drawO(int timeMs) {
    unsigned long endTime = millis() + timeMs;
    uint8_t x = 0;
    uint8_t y = 0;
    while (millis() < endTime) {
        if (x != 1 && y != 1) {
            digitalWrite(LED_PIN_X[x], HIGH);
            digitalWrite(LED_PIN_Y[y], HIGH);
            delay(SCAN_TIME);

            x++;
            if (x > 2) {
                x = 0;
                y++;
            }
            if (y > 2) {
                x = 0;
                y = 0;
            }
        }
        // digitalWrite(LED_PIN_X[0], HIGH);
        // digitalWrite(LED_PIN_Y[0], HIGH);
        // delay(SCAN_TIME);
        // digitalWrite(LED_PIN_Y[0], LOW);
        // digitalWrite(LED_PIN_Y[1], HIGH);
        // delay(SCAN_TIME);
        // digitalWrite(LED_PIN_Y[1], LOW);
        // digitalWrite(LED_PIN_Y[2], HIGH);
    }
}