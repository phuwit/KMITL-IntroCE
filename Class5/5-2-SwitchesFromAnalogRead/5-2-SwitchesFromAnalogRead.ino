const uint8_t BUTTON_PIN = A0;
const uint8_t LEDS_PIN[4] = {11, 10, 9, 8};

void turnOffAllPin();

void setup() {
    Serial.begin(9600);

    pinMode(BUTTON_PIN, INPUT);
    for (int i = 0; i < 4; i++) {
        pinMode(LEDS_PIN[i], OUTPUT);
    } 
}

void loop() {
    Serial.println(analogRead(BUTTON_PIN));
    // 1 978
    // 2 931
    // 3 510
    // 4 1002

    int readResult = analogRead(BUTTON_PIN);
    if (readResult > 992 && readResult < 1022) {
        turnOffAllPin();
        digitalWrite(LEDS_PIN[3], HIGH);
    }
    else if (readResult > 958 && readResult < 998) {
        turnOffAllPin();
        digitalWrite(LEDS_PIN[0], HIGH);
    }
    else if (readResult > 921 && readResult < 951) {
        turnOffAllPin();
        digitalWrite(LEDS_PIN[1], HIGH);
    }
    else if (readResult > 490 && readResult < 530) {
        turnOffAllPin();
        digitalWrite(LEDS_PIN[2], HIGH);
    }
    else {
        turnOffAllPin();
    }
}

void turnOffAllPin() {
    for (int i = 0; i < 4; i++) {
        digitalWrite(LEDS_PIN[i], LOW);
    }
}