const uint8_t NUM_PINS = 8;
// a,b,c,...,g,DP segments
const uint8_t SEGMENT_PINS[NUM_PINS] = {2, 3, 4, 5, 6, 7, 8, 9};
// input number to light up from array index then get a,b,c,... segments in index number from SEGMENT_PIN to map into pin number
const uint8_t SEGMENT_TABLE[10][7] = {
//   a  b  c  d  e  f  g
    {0, 0, 0, 0, 0, 0, 1}, // 0
    {1, 0, 0, 1, 1, 1, 1}, // 1
    {0, 0, 1, 0, 0, 1, 0}, // 2
    {0, 0, 0, 0, 1, 1, 0}, // 3
    {1, 0, 0, 1, 1, 0, 0}, // 4
    {0, 1, 0, 0, 1, 0, 0}, // 5
    {0, 1, 0, 0, 0, 0, 0}, // 6
    {0, 0, 0, 1, 1, 1, 1}, // 7
    {0 ,0 ,0 ,0 ,0 ,0 ,0}, // 8
    {0 ,0 ,0 ,0 ,1 ,0 ,0}  // 9

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
            Serial.println("is int");

            for (int i = 0; i < 7; i++) {
                // Serial.println("new loop");
                // Serial.println(i);
                // Serial.println(SEGMENT_PINS[i]);
                // Serial.println(SEGMENT_TABLE[incomingByteAsInt][i]);
                digitalWrite(SEGMENT_PINS[i], SEGMENT_TABLE[incomingByteAsInt][i]);
            }
        }
        else  {
            Serial.println("Input is not number");
        }
    }
}