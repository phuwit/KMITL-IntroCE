const uint8_t DATA_TRANSMIT_PIN = 2;

void setup() {
    pinMode(DATA_TRANSMIT_PIN, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    if (Serial.available() > 0) {
        int input = Serial.read() - '0';
        if (input >= '0' && input <= '9'){
            Serial.println(input);
            for (int i = 0; i < input; i++) {
                digitalWrite(DATA_TRANSMIT_PIN, HIGH);
                delay(100);
                digitalWrite(DATA_TRANSMIT_PIN, LOW);
                delay(100);
            }

            delay(300);
        }
    }
}