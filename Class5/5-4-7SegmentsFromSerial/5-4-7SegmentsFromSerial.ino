const uint8_t NUM_PINS = 8;
// a,b,c,...,g,DP segments
const uint8_t SEGMENT_PINS[NUM_PINS] = {2, 3, 4, 5, 6, 7, 8, 9};
// input number to light up from array index then get a,b,c,... segments in index number from SEGMENT_PIN to map into pin number
const uint8_t SEGMENT_TABLE[10][7] = {
//   a  b  c  d  e  f  g
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 0}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}, // 9
};

void setup() {
    Serial.begin(9600);
    for (int i = 0; i < NUM_PINS; i++) {
        pinMode(SEGMENT_PINS[i], OUTPUT);
    }
}

void loop() {
    if (Serial.available() > 0) {
        Serial.print("Enter a number : ");
        char incomingByte = Serial.read();
        int incomingByteAsInt = incomingByte - 48;
        Serial.println(incomingByte);
        
        if (incomingByteAsInt >= 0 && incomingByteAsInt <= 9) {
            for (int i = 0; i < 7; i++) {
                digitalWrite(SEGMENT_PINS[i], SEGMENT_TABLE[incomingByteAsInt][i]);
            }
        }
        else if (incomingByte != 10)  {
            Serial.println("Input is not number");
        }

        delay(500);
    }
}