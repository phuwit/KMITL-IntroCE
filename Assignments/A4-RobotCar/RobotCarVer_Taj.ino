#include "ST7735_TEE.h"

#define white false
#define black true
#define LDR_PIN A5
int ldrBaseValue = 0;
const short SENSORS_COUNT = 5;
enum SensorName { LL,
                  L,
                  C,
                  R,
                  RR };
const uint8_t SENSORS_PIN[SENSORS_COUNT] = { A0, A1, A2, A3, A4 };

bool sensorsReading[SENSORS_COUNT];
bool prevoiusSensorsReading[SENSORS_COUNT] = { false };
const int SENSOR_THRESHOLD = 300;

int offCourse = 0;
SensorName lastExtreme;

const int enableRight = 3;  // right
const int rightBackward = 2;
const int rightForward = 7;
const int enableLeft = 6;  // left
const int leftBackward = 4;
const int leftForward = 5;

const unsigned int BASE_POWER = 255 * 0.35;
const unsigned int TURN_POWER = 255 * 0.35;
const unsigned int HARD_TURN_POWER = 255 * 0.4;


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
  pinMode(LDR_PIN, INPUT);
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

  if (offCourse > 10) {
    forward(BASE_POWER, 100);
    stop();
    sensors_read(true);

    while (offCourse) {
      sensors_read(false);
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
    // sensors_restore();
    sensors_read(true);
  }
  int directionLastMove = 0;
  if (sensorsReading[SensorName::RR] && sensorsReading[SensorName::LL] && sensorsReading[SensorName::R] && sensorsReading[SensorName::L] && sensorsReading[SensorName::C]) {
    //debug
    //actual code
    stop();
    delay(500);
    int blinks = 0;
    unsigned long startTime = millis();
    bool lastLdrVal = analogRead(A5) < 350;
    //count down fivesec
    while (millis() - startTime < 5000) {
      bool ldrVal = analogRead(A5) < 350;
      // Serial.println(analogRead(LDR_PIN));
      // digitalWrite(LED_BUILTIN, ldrVal);
      if (ldrVal > lastLdrVal) {
        blinks++;
      }
      lastLdrVal = ldrVal;
    }
    if (blinks <= 1) {
      // forward(BASE_POWER, 150);
      // turn_left(HARD_TURN_POWER, 400);
      // stop();

      while (sensorsReading[SensorName::RR] == black && sensorsReading[SensorName::LL] == black ) {
        forward(BASE_POWER, 100);
        stop();
        sensors_read(true);
      }

      while (sensorsReading[SensorName::L] == black) {
        turn_left(TURN_POWER, 20);
        stop();
        sensors_read(true);
      }

      turn_right(TURN_POWER, 50);
      forward(TURN_POWER, 100);
      stop();
    } else {
      // forward(BASE_POWER, 150);
      // turn_right(HARD_TURN_POWER, 400);
      // stop();

      while (sensorsReading[SensorName::RR] == black && sensorsReading[SensorName::LL] == black ) {
        forward(BASE_POWER, 100);
        stop();
        sensors_read(true);
      }

      while (sensorsReading[SensorName::R] == black) {
        turn_right(TURN_POWER, 20);
        stop();
        sensors_read(true);
      }

      turn_left(TURN_POWER, 50);
      forward(TURN_POWER, 100);
      stop();
    }

    //the code below handle moom laem
  } else if (sensorsReading[SensorName::RR] && sensorsReading[SensorName::C] && !sensorsReading[SensorName::R]) {
    while (offCourse < 2) {
      forward(BASE_POWER, 100);
      stop();
      sensors_read(true);
    }
    while (!(sensorsReading[SensorName::R])) {
      turn_right(TURN_POWER, 20);
      stop();
      sensors_read(true);
    }
    stop();
    turn_left(TURN_POWER, 50);
    forward(TURN_POWER, 100);
    directionLastMove = 1;
  } else if (sensorsReading[SensorName::LL] && sensorsReading[SensorName::C] && !sensorsReading[SensorName::L]) {
    while (offCourse < 2) {
      forward(BASE_POWER, 100);
      stop();
      sensors_read(true);
    }
    while (!(sensorsReading[SensorName::L])) {
      turn_left(TURN_POWER, 20);
      stop();
      sensors_read(true);
    }
    stop();
    turn_right(TURN_POWER, 50);
    forward(TURN_POWER, 100);
    directionLastMove = 1;
  } else if (sensorsReading[SensorName::RR]) {
    forward(BASE_POWER, 20);
    turn_right(HARD_TURN_POWER, 60);
    stop();
    lastExtreme = SensorName::RR;
    directionLastMove = 1;
  } else if (sensorsReading[SensorName::LL]) {
    forward(BASE_POWER, 20);
    turn_left(HARD_TURN_POWER, 60);
    stop();
    lastExtreme = SensorName::LL;
    directionLastMove = 1;
  } else if (sensorsReading[SensorName::R]) {
    forward(BASE_POWER, 20);
    turn_right(TURN_POWER, 60);
    stop();
    directionLastMove = 1;
  } else if (sensorsReading[SensorName::L]) {
    forward(BASE_POWER, 20);
    turn_left(TURN_POWER, 60);
    stop();
    directionLastMove = 1;
  } else if (sensorsReading[SensorName::C]) {
    forward(BASE_POWER, 60);
    stop();
    directionLastMove = 0;
  } else {
    forward(BASE_POWER, 60);
    stop();
    directionLastMove = 1;
  }
  // ldrBaseValue += analogRead(LDR_PIN);
  // ldrBaseValue /= 2;
}

// void loop() {
//     forward(150, 200);
//     turn_left(150, 200);
//     turn_right(150, 200);
// }

void sensors_read(bool save) {
  for (int i = 0; i < SENSORS_COUNT; i++) {
    if (analogRead(SENSORS_PIN[i]) <= SENSOR_THRESHOLD) {
      sensorsReading[i] = white;
    } else {
      sensorsReading[i] = black;
    }
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