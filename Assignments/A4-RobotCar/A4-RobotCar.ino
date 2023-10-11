#define white false
#define black true

const short SENSORS_COUNT = 5;
enum SensorName { LL, L, C, R, RR };
const uint8_t SENSORS_PIN[SENSORS_COUNT] = {A0, A1, A2, A3, A4};
bool sensorsReading[SENSORS_COUNT];
bool prevoiusSensorsReading[SENSORS_COUNT] = {false};
const int SENSOR_THRESHOLD = 300;

int offCourse = 0;

const int enableRight = 3;  // right
const int rightBackward = 2;
const int rightForward = 7;
const int enableLeft = 6;  // left
const int leftBackward = 4;
const int leftForward = 5;

void sensors_read(bool save);
void sensors_save();
void sensors_restore();
void stop();
void forward(int speed, int time);
void backward(int speed, int time);
void turn_right(int speed, int time);
void turn_left(int speed, int time);

void setup() {
    Serial.begin(9600);
    for (int i = 0; i < SENSORS_COUNT; i++) {
        pinMode(SENSORS_PIN[i], INPUT);
    }

    pinMode(enableRight, OUTPUT);
    pinMode(rightBackward, OUTPUT);
    pinMode(rightForward, OUTPUT);
    pinMode(enableLeft, OUTPUT);
    pinMode(leftBackward, OUTPUT);
    pinMode(leftForward, OUTPUT);

    delay(1000);
}

void loop() {
    sensors_read(true);

    if (offCourse > 50) {
        forward(150, 100);
        stop();
        sensors_read(true);

        while (offCourse) {
            sensors_read(false);
            backward(150, 100);
        }

        backward(150, 100);
        stop();
        // sensors_restore();
        sensors_read(true);
    }

    if (sensorsReading[SensorName::RR] && sensorsReading[SensorName::R]) {
        forward(100, 50);
        turn_right(200, 130);
        stop();
    } else if (sensorsReading[SensorName::LL] && sensorsReading[SensorName::L]) {
        forward(100, 50);
        turn_left(200, 130);
        stop();
    } else if (sensorsReading[SensorName::RR]) {
        forward(100, 20);
        turn_right(150, 100);
        stop();
    } else if (sensorsReading[SensorName::LL]) {
        forward(100, 20);
        turn_left(150, 100);
        stop();
    } else if (sensorsReading[SensorName::R]) {
        forward(100, 20);
        turn_right(100, 100);
        stop();
    } else if (sensorsReading[SensorName::L]) {
        forward(100, 20);
        turn_left(100, 100);
        stop();
    } else if (sensorsReading[SensorName::C]) {
        forward(150, 100);
        stop();
    }
}

// void loop() {
//     forward(150, 200);
//     turn_left(150, 200);
//     turn_right(150, 200);
// }

void sensors_read(bool save) {
    for (int i = 0; i < SENSORS_COUNT; i++) {
        if (analogRead(SENSORS_PIN[i]) <= SENSOR_THRESHOLD)
            sensorsReading[i] = white;
        else
            sensorsReading[i] = black;

        Serial.print(sensorsReading[i]);
        Serial.print("\t");
    }
    Serial.println("");

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
    
    if (save) sensors_save();
}

void sensors_save() {
    if (offCourse == 0) {
        for (int i = 0; i < SENSORS_COUNT; i++) {
            prevoiusSensorsReading[i] = sensorsReading[i];
        }
    }
}

void sensors_restore() {
    for (int i = 0; i < SENSORS_COUNT; i++) {
        sensorsReading[i] = prevoiusSensorsReading[i];
    }
}

void stop() {
  analogWrite(enableLeft, 0);
  analogWrite(enableRight, 0);
}


void forward(int speed, int time) {
    analogWrite(enableRight, speed - (0.16 * speed));
    analogWrite(enableLeft, speed);
    digitalWrite(rightBackward, LOW);
    digitalWrite(rightForward, HIGH);
    digitalWrite(leftBackward, LOW);
    digitalWrite(leftForward, HIGH);
    delay(time);
}

void backward(int speed, int time) {
    analogWrite(enableRight, speed - (0.16 * speed));
    analogWrite(enableLeft, speed);
    digitalWrite(rightBackward, HIGH);
    digitalWrite(rightForward, LOW);
    digitalWrite(leftBackward, HIGH);
    digitalWrite(leftForward, LOW);
    delay(time);
}

void turn_right(int speed, int time) {
    analogWrite(enableLeft, speed);
    digitalWrite(leftBackward, LOW);
    digitalWrite(leftForward, HIGH);
    analogWrite(enableRight, speed - (0.16 * speed));
    digitalWrite(rightBackward, HIGH);
    digitalWrite(rightForward, LOW);
    delay(time);
}

void turn_left(int speed, int time) {
    analogWrite(enableRight, speed - (0.16 * speed));
    digitalWrite(rightBackward, LOW);
    digitalWrite(rightForward, HIGH);
    analogWrite(enableLeft, speed);
    digitalWrite(leftBackward, HIGH);
    digitalWrite(leftForward, LOW);
    delay(time);
}