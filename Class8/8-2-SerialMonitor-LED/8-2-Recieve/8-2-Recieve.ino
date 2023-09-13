const uint8_t LED_PIN = 13;
const uint8_t DATA_RECIEVE_PIN = 3;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(DATA_RECIEVE_PIN, INPUT);
}

void loop() {
    digitalWrite(LED_PIN, digitalRead(DATA_RECIEVE_PIN));
}