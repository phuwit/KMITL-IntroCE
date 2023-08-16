const uint8_t NUM_PINS = 8;
// a,b,c,...,g,DP segments
const uint8_t SEGMENT_PINS[NUM_PINS] = {2, 3, 4, 5, 6, 7, 8, 9};
// input number to light up from array index then get a,b,c,... segments in
// index number from SEGMENT_PIN to map into pin number
const uint8_t SEGMENT_TABLE[12][9] = {
//   a  b  c  d  e  f  g
    {0, 0, 0, 0, 0, 0, 1},  // 0
    {1, 0, 0, 1, 1, 1, 1},  // 1
    {0, 0, 1, 0, 0, 1, 0},  // 2
    {0, 0, 0, 0, 1, 1, 0},  // 3
    {1, 0, 0, 1, 1, 0, 0},  // 4
    {0, 1, 0, 0, 1, 0, 0},  // 5
    {0, 1, 0, 0, 0, 0, 0},  // 6
    {0, 0, 0, 1, 1, 1, 1},  // 7
    {0, 0, 0, 0, 0, 0, 0},  // 8
    {0, 0, 0, 0, 1, 0, 0},  // 9
    {0, 1, 1, 1, 0, 0, 1},  // More
    {1, 1, 1, 0, 0, 0, 1},  // Less
};

int displayNum = 1;
int randomNum;
int numBeforeGuess = -1;

const uint8_t BUTTON_G_PIN = 13;
const uint8_t BUTTON_A_PIN = 12;

// debounce stuffs
// Variables will change:
int buttonGState;            // the current reading from the input pin
int buttonAState;            // the current reading from the input pin
int lastButtonGState = HIGH;  // the previous reading from the input pin
int lastButtonAState = HIGH;  // the previous reading from the input pin

int lastReadableButtonGState = LOW;
int lastReadableButtonAState = LOW;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an
// int.
unsigned long lastDebounceTimeButtonG =
    0;  // the last time the output pin was toggled
unsigned long lastDebounceTimeButtonA =
    0;  // the last time the output pin was toggled
const unsigned long debounceDelay =
    50;  // the debounce time; increase if the output flickers


int deBounce(uint8_t &buttonPin, int &buttonState, int &lastButtonState,
             unsigned long &lastDebounceTime);


void setup() {
    Serial.begin(9600);
    randomSeed(analogRead(0));
    randomNum = random(1, 10);

    for (int i = 0; i < NUM_PINS; i++) {
        pinMode(SEGMENT_PINS[i], OUTPUT);
    }

    // internal pullups
    pinMode(BUTTON_G_PIN, INPUT_PULLUP);
    pinMode(BUTTON_A_PIN, INPUT_PULLUP);
}

void loop() {
    int readableButtonStateA =
        deBounce(BUTTON_A_PIN, buttonAState, lastButtonAState, lastDebounceTimeButtonA);
    int readableButtonStateG =
        deBounce(BUTTON_G_PIN, buttonGState, lastButtonGState, lastDebounceTimeButtonG);
    
    // Serial.print(readableButtonStateA);
    // Serial.println(readableButtonStateG);

    if (readableButtonStateA == HIGH &&
        readableButtonStateA != lastReadableButtonAState) {
            if (numBeforeGuess != -1) {
                displayNum = numBeforeGuess;
                numBeforeGuess = -1;
            }
            
            displayNum++;
            if (displayNum >= 10) {
                displayNum = 1;
            }
            Serial.print("pressed increment displayNum is now");
            Serial.println(displayNum);
    }
    if (readableButtonStateG == HIGH &&
        readableButtonStateG != lastReadableButtonGState) {
            numBeforeGuess = displayNum;

            Serial.print("pressed guess, randomNum");
            Serial.println(randomNum);

            if(displayNum == randomNum) {
                Serial.println("correct");
                displayNum = 0;
                randomNum = random(1, 10);
            }
            else if(displayNum > randomNum) {
                Serial.println("should be less");
                displayNum = 10;
            }
            else if(displayNum < randomNum) {
                Serial.println("should be more");
                displayNum = 11;
            }
    }

    for (int i = 0; i < 7; i++) {
        digitalWrite(SEGMENT_PINS[i], SEGMENT_TABLE[displayNum][i]);
    }
}

int deBounce(const uint8_t &buttonPin, int &buttonState, int &lastButtonState,
             unsigned long &lastDebounceTime) {
    int readableButtonState = LOW;

    // read the state of the switch into a local variable:
    int reading = digitalRead(buttonPin);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState) {
        // reset the debouncing timer
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        // whatever the reading is at, it's been there for longer than the
        // debounce delay, so take it as the actual current state:

        // if the button state has changed:
        if (reading != buttonState) {
            buttonState = reading;

            // only toggle the LED if the new button state is HIGH
            if (buttonState == HIGH) {
                readableButtonState = HIGH;
            }
        }
    }

    // save the reading. Next time through the loop, it'll be the
    // lastButtonState:
    lastButtonState = reading;

    return readableButtonState;
}