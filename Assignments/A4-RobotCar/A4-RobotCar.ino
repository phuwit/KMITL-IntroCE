#define white false
#define black true
#define LDR_PIN A5
const short SENSORS_COUNT = 5;
enum SensorName { LL, L, C, R, RR };
const uint8_t SENSORS_PIN[SENSORS_COUNT] = {A0, A1, A2, A3, A4};

bool sensorsReading[SENSORS_COUNT];
bool prevoiusSensorsReading[SENSORS_COUNT] = {false};
const int SENSOR_THRESHOLD = 300;

int offCourse = 0;
SensorName lastExtreme;

const uint8_t enableRight = 9;  // right
const uint8_t rightBackward = 8;
const uint8_t rightForward = 11;
const uint8_t enableLeft = 10;  // left
const uint8_t leftBackward = 12;
const uint8_t leftForward = 13;

const uint8_t switchPin = 3;

const unsigned int BASE_POWER = 255 * 0.4;
const unsigned int TURN_POWER = 255 * 0.3;
const unsigned int HARD_TURN_POWER = 255 * 0.4;

// const float LEFT_OFFSET = 0.16;
const float LEFT_OFFSET = 0.12;
const float RIGHT_OFFSET = 0.0;

int hardTurnsAfterIntersectionCount = 0;
unsigned int timestampAtIntersection = millis();
SensorName nextIntersectionSensorToCheck;

void sensors_read();
void stop();
void forward(int speed, int time);
void backward(int speed, int time);
void turn_right(int speed, int time);
void turn_left(int speed, int time);
void handleIntersection();
void handleSwitch();
void (*reset)(void) = 0;  // create a standard reset function
bool isFinishLoop = false;

void setup() {
    Serial.begin(9600);
    for (int i = 0; i < SENSORS_COUNT; i++) {
        pinMode(SENSORS_PIN[i], INPUT);
    }
    pinMode(LDR_PIN, INPUT);
    pinMode(enableRight, OUTPUT);
    pinMode(rightBackward, OUTPUT);
    pinMode(rightForward, OUTPUT);
    pinMode(enableLeft, OUTPUT);
    pinMode(leftBackward, OUTPUT);
    pinMode(leftForward, OUTPUT);
    pinMode(switchPin, INPUT);

    attachInterrupt(digitalPinToInterrupt(switchPin), handleSwitch, CHANGE);

    delay(1000);
}

void loop() {
    if (digitalRead(switchPin)) {
        sensors_read();

        if (offCourse > 20) {
            forward(BASE_POWER, 100);
            stop();
            sensors_read();

            while (offCourse) {
                sensors_read();
                backward(BASE_POWER, 100);
            }

            backward(BASE_POWER, 100);
            stop();

            if (sensorsReading[SensorName::RR]) {
                forward(BASE_POWER, 20);
                turn_right(HARD_TURN_POWER, 60);
                stop();
            } else if (sensorsReading[SensorName::LL]) {
                forward(BASE_POWER, 20);
                turn_left(HARD_TURN_POWER, 60);
                stop();
            }
            sensors_read();
        }

        if (hardTurnsAfterIntersectionCount <= 0 && 
            sensorsReading[SensorName::RR] && sensorsReading[SensorName::LL] &&
            sensorsReading[SensorName::R] && sensorsReading[SensorName::L] &&
            sensorsReading[SensorName::C]) {
            // debug
            // actual code
            handleIntersection();
            hardTurnsAfterIntersectionCount = 1;
            // the code below handle moom laem
        } else if (hardTurnsAfterIntersectionCount >= 3 &&
                   ((millis() - timestampAtIntersection) > 4000))
                    // && !isFinishLoop)
        {
            // if (sensorsReading[nextIntersectionSensorToCheck]) {
            if (sensorsReading[SensorName::LL] ||
                sensorsReading[SensorName::RR]) {
                handleIntersection();
                hardTurnsAfterIntersectionCount = 0;
                // isFinishLoop = true;
                
            }
        }

        if (sensorsReading[SensorName::RR] && sensorsReading[SensorName::C] &&
            !sensorsReading[SensorName::R]) {
            while (offCourse < 2) {
                forward(BASE_POWER, 10);
                stop();
                sensors_read();
            }
            while (!(sensorsReading[SensorName::R])) {
                turn_right(HARD_TURN_POWER, 10);
                stop();
                sensors_read();
            }
            stop();
            turn_left(TURN_POWER, 50);
            forward(TURN_POWER, 300);
        } else if (sensorsReading[SensorName::LL] &&
                   sensorsReading[SensorName::C] &&
                   !sensorsReading[SensorName::L]) {
            while (offCourse < 2) {
                forward(BASE_POWER, 10);
                stop();
                sensors_read();
            }
            while (!(sensorsReading[SensorName::L])) {
                turn_left(HARD_TURN_POWER, 10);
                stop();
                sensors_read();
            }
            stop();
            turn_right(TURN_POWER, 50);
            forward(TURN_POWER, 300);
        } else if (sensorsReading[SensorName::RR]) {
            forward(BASE_POWER, 5);
            turn_right(HARD_TURN_POWER, 10);
            stop();
            lastExtreme = SensorName::RR;

            if (hardTurnsAfterIntersectionCount >= 1)
                hardTurnsAfterIntersectionCount++;
        } else if (sensorsReading[SensorName::LL]) {
            forward(BASE_POWER, 5);
            turn_left(HARD_TURN_POWER, 10);
            stop();
            lastExtreme = SensorName::LL;

            if (hardTurnsAfterIntersectionCount >= 1)
                hardTurnsAfterIntersectionCount++;
        } else if (sensorsReading[SensorName::R]) {
            forward(BASE_POWER, 5);
            turn_right(TURN_POWER, 10);
            stop();
        } else if (sensorsReading[SensorName::L]) {
            forward(BASE_POWER, 5);
            turn_left(TURN_POWER, 10);
            stop();
        } else if (sensorsReading[SensorName::C]) {
            forward(BASE_POWER, 10);
            stop();
        } else {
            forward(BASE_POWER, 10);
            stop();
        }
        stop();
    } else {
        hardTurnsAfterIntersectionCount = 0;
        stop();
        reset();
    }
}

// void loop() {
//     // sensors_read();
//     forward(BASE_POWER, 200);
//     // turn_left(TURN_POWER, 200);
//     // turn_right(TURN_POWER, 200);
// }

void sensors_read() {
    for (int i = 0; i < SENSORS_COUNT; i++) {
        if (analogRead(SENSORS_PIN[i]) <= SENSOR_THRESHOLD) {
            sensorsReading[i] = white;
        } else {
            sensorsReading[i] = black;
        }
        // Serial.print(analogRead(SENSORS_PIN[i]));
        // Serial.print("\t");
    }
    // Serial.println("");

    offCourse++;
    for (int i = 0; i < SENSORS_COUNT; i++) {
        // if (i == SensorName::C) {
        //     continue;
        // }
        if (sensorsReading[i] == black) {
            offCourse = 0;
            break;
        }
    }
}

void stop() {
    analogWrite(enableLeft, 0);
    analogWrite(enableRight, 0);
}

void forward(int speed, int time) {
    analogWrite(enableRight, speed - (RIGHT_OFFSET * speed));
    analogWrite(enableLeft, speed - (LEFT_OFFSET * speed));
    digitalWrite(rightBackward, LOW);
    digitalWrite(rightForward, HIGH);
    digitalWrite(leftBackward, LOW);
    digitalWrite(leftForward, HIGH);
    delay(time);
}

void backward(int speed, int time) {
    analogWrite(enableRight, speed - (RIGHT_OFFSET * speed));
    analogWrite(enableLeft, speed - (LEFT_OFFSET * speed));
    digitalWrite(rightBackward, HIGH);
    digitalWrite(rightForward, LOW);
    digitalWrite(leftBackward, HIGH);
    digitalWrite(leftForward, LOW);
    delay(time);
}

void turn_right(int speed, int time) {
    analogWrite(enableLeft, speed - (LEFT_OFFSET * speed));
    digitalWrite(leftBackward, LOW);
    digitalWrite(leftForward, HIGH);
    analogWrite(enableRight, speed - (RIGHT_OFFSET * speed));
    digitalWrite(rightBackward, HIGH);
    digitalWrite(rightForward, LOW);
    delay(time);
}

void turn_left(int speed, int time) {
    analogWrite(enableRight, speed - (RIGHT_OFFSET * speed));
    digitalWrite(rightBackward, LOW);
    digitalWrite(rightForward, HIGH);
    analogWrite(enableLeft, speed - (LEFT_OFFSET * speed));
    digitalWrite(leftBackward, HIGH);
    digitalWrite(leftForward, LOW);
    delay(time);
}

void handleIntersection() {
    stop();
    delay(500);
    int blinks = 0;
    unsigned long startTime = millis();
    const unsigned int baseLdrValue = analogRead(LDR_PIN) / 2 + 100;
    bool lastLdrValue = analogRead(LDR_PIN) < baseLdrValue;
    // count down fivesec
    while (millis() - startTime < 5000) {
        bool ldrValue = analogRead(LDR_PIN) < baseLdrValue;
        if (ldrValue > lastLdrValue) {
                blinks++;
            }
        lastLdrValue = ldrValue;
    }

    // const unsigned int baseLdrValue = analogRead(LDR_PIN);
    // bool lastLdrValue = analogRead(LDR_PIN) < baseLdrValue;
    // // count down fivesec
    // while (millis() - startTime < 5000) {
    //     bool ldrValue = analogRead(LDR_PIN) < baseLdrValue;
    //     if (ldrValue > lastLdrValue) {
    //             blinks++;
    //         }
    //     lastLdrValue = ldrValue;
    // }

    // while (sensorsReading[SensorName::RR] == black ||
    //         sensorsReading[SensorName::LL] == black) {
    //     forward(BASE_POWER, 100);
    //     stop();
    //     sensors_read();
    // }

    if (hardTurnsAfterIntersectionCount >= 3) {
        forward(BASE_POWER, 500);
    } else {
        forward(BASE_POWER, 300);
    }

    // while (sensorsReading[SensorName::L] == white ||
    //        sensorsReading[SensorName::R] == white) {
    //         forward(BASE_POWER, 20);
    //         stop();
    //         sensors_read();
    // }

    if (blinks <= 1) {
        // forward(BASE_POWER, 150);
        // turn_left(HARD_TURN_POWER, 350);
        // stop();
        nextIntersectionSensorToCheck = SensorName::LL;

        turn_left(HARD_TURN_POWER, 350);

        while (sensorsReading[SensorName::L] == white) {
            turn_left(TURN_POWER, 20);
            stop();
            sensors_read();
        }

        // turn_right(TURN_POWER, 50);
        forward(TURN_POWER, 100);
        stop();
    } else {
        // forward(BASE_POWER, 150);
        // turn_right(HARD_TURN_POWER, 350);
        // stop();

        turn_right(HARD_TURN_POWER, 350);

        nextIntersectionSensorToCheck = SensorName::RR;

        while (sensorsReading[SensorName::R] == white) {
            turn_right(TURN_POWER, 20);
            stop();
            sensors_read();
        }

        // turn_left(TURN_POWER, 50);
        forward(TURN_POWER, 100);
        stop();
    }

    // // turn off going straight
    // Serial.println("turn off going straight");
    // while (!(sensorsReading[SensorName::RR] &&
    // sensorsReading[SensorName::LL] && sensorsReading[SensorName::R]
    // && sensorsReading[SensorName::L] &&
    // sensorsReading[SensorName::C])) {
    //     sensors_read();
    //     if (sensorsReading[SensorName::R] ||
    //     sensorsReading[SensorName::RR]) {
    //         forward(BASE_POWER, 20);
    //         turn_right(TURN_POWER, 60);
    //         stop();
    //     } else {
    //         forward(BASE_POWER, 20);
    //         turn_left(TURN_POWER, 60);
    //         stop();
    //     }
    // }

    // Serial.println("back to normal");
    timestampAtIntersection = millis();
}

void handleSwitch() {
    Serial.println("Switch triggered");
    reset();
}