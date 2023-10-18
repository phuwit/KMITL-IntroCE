#include "ST7735_TEE.h"

#define white false
#define black true
#define LDR_PIN A5
int ldrBaseValue = 0;
const short SENSORS_COUNT = 5;
enum SensorName { LL, L, C, R, RR };
const uint8_t SENSORS_PIN[SENSORS_COUNT] = {A0, A1, A2, A3, A4};

bool sensorsReading[SENSORS_COUNT];
bool prevoiusSensorsReading[SENSORS_COUNT] = {false};
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

TEE_ST7735 lcd(9, 10, 11, 12, 13);  // Arduino  csk,sda,A0,rst,cs

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

    lcd.init(lcd.VERTICAL);
    lcd.fillScreen(WHITE);
    lcd.drawString(10, 10, "hello world", BLACK, 1);
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
            lcd.fillRect(120, 50, 5, 5, GREEN);
            forward(BASE_POWER, 20);
            turn_right(HARD_TURN_POWER, 60);
            stop();
            lcd.fillRect(120, 50, 5, 5, WHITE);
        } else if (sensorsReading[SensorName::LL]) {
            lcd.fillRect(10, 50, 5, 5, GREEN);
            forward(BASE_POWER, 20);
            turn_left(HARD_TURN_POWER, 60);
            stop();
            lcd.fillRect(10, 50, 5, 5, WHITE);
        }
        // sensors_restore();
        sensors_read(true);
    }

    // if (sensorsReading[SensorName::RR] && sensorsReading[SensorName::R]) {
    //     lcd.fillRect(10, 50, 5, 5, GREEN);
    //     forward(100, 50);
    //     turn_right(200, 130);
    //     stop();
    //     lcd.fillRect(10, 50, 5, 5, WHITE);
    // } else if (sensorsReading[SensorName::LL] &&
    // sensorsReading[SensorName::L]) {
    //     lcd.fillRect(120, 50, 5, 5, GREEN);
    //     forward(100, 50);
    //     turn_left(200, 130);
    //     stop();
    //     lcd.fillRect(120, 50, 5, 5, WHITE);
    if (sensorsReading[SensorName::RR] && sensorsReading[SensorName::LL] &&
        sensorsReading[SensorName::R] && sensorsReading[SensorName::L] &&
        sensorsReading[SensorName::C]) {
        // debug
        lcd.fillRect(0, 0, 5000, 5000, GREEN);
        // actual code
        stop();
        delay(500);
        int blinks = 0;
        unsigned long startTime = millis();
        bool lastldrValue = analogRead(LDR_PIN) < 250;
        // count down fivesec
        while (millis() - startTime < 5000) {
            bool ldrValue = analogRead(LDR_PIN) < 250;
            // Serial.println(analogRead(LDR_PIN));
            // digitalWrite(LED_BUILTIN, ldrValue);
            if (ldrValue > lastldrValue) {
                blinks++;
            }
            lastldrValue = ldrValue;
        }
        if (blinks <= 1) {
            // turn left
            forward(100, 50);
            turn_left(200, 130);
            stop();
            return;
        } else {
            // turn right
            forward(100, 50);
            turn_right(200, 130);
            stop();
        }
        lcd.fillRect(0, 0, 5000, 5000, WHITE);

    } else if (sensorsReading[SensorName::RR]) {
        forward(100, 50);
        while (!(
            sensorsReading[SensorName::RR] && sensorsReading[SensorName::LL] &&
            sensorsReading[SensorName::R] && sensorsReading[SensorName::L])) {
            turn_right(100, 20);
            stop();
        }
    } else if (sensorsReading[SensorName::LL]) {
<<<<<<< Updated upstream
        forward(100, 50);
        while (!(
            sensorsReading[SensorName::RR] && sensorsReading[SensorName::LL] &&
            sensorsReading[SensorName::R] && sensorsReading[SensorName::L])) {
            turn_left(100, 20);
            stop();
        }
    } else if (sensorsReading[SensorName::R]) {
        lcd.fillRect(30, 50, 5, 5, GREEN);
        forward(BASE_POWER, 20);
        turn_right(TURN_POWER, 60);
        stop();
        lcd.fillRect(20, 50, 5, 5, WHITE);
    } else if (sensorsReading[SensorName::L]) {
        lcd.fillRect(90, 50, 5, 5, GREEN);
        forward(BASE_POWER, 20);
        turn_left(TURN_POWER, 60);
        stop();
        lcd.fillRect(90, 50, 5, 5, WHITE);
    } else if (sensorsReading[SensorName::C]) {
        lcd.fillRect(60, 50, 5, 5, GREEN);
        forward(BASE_POWER, 60);
        stop();
        lcd.fillRect(60, 50, 5, 5, WHITE);
    } else {
        lcd.fillRect(60, 50, 5, 5, GREEN);
        forward(BASE_POWER, 60);
        stop();
        lcd.fillRect(60, 50, 5, 5, WHITE);
    }
    ldrBaseValue += analogRead(LDR_PIN);
    ldrBaseValue /= 2;
=======
      lcd.fillRect(10, 50, 5, 5, GREEN);
      forward(BASE_POWER, 20);
      turn_left(HARD_TURN_POWER, 60);
      stop();
      lcd.fillRect(10, 50, 5, 5, WHITE);
    }
    // sensors_restore();
    sensors_read(true);
  }
  int directionLastMove = 0;

  // if (sensorsReading[SensorName::RR] && sensorsReading[SensorName::R]) {
  //     lcd.fillRect(10, 50, 5, 5, GREEN);
  //     forward(100, 50);
  //     turn_right(200, 130);
  //     stop();
  //     lcd.fillRect(10, 50, 5, 5, WHITE);
  // } else if (sensorsReading[SensorName::LL] && sensorsReading[SensorName::L]) {
  //     lcd.fillRect(120, 50, 5, 5, GREEN);
  //     forward(100, 50);
  //     turn_left(200, 130);
  //     stop();
  //     lcd.fillRect(120, 50, 5, 5, WHITE);
  if (sensorsReading[SensorName::RR] && sensorsReading[SensorName::LL] && sensorsReading[SensorName::R] && sensorsReading[SensorName::L] && sensorsReading[SensorName::C] && directionLastMove == 0) {
    //debug
    lcd.fillRect(0, 0, 5000, 5000, GREEN);
    //actual code
    stop();
    delay(500);
    int blinks = 0;
    unsigned long startTime = millis();
    bool lastldrValue = analogRead(LDR_PIN) < 250;
    //count down fivesec
    while (millis() - startTime < 5000) {
      bool ldrValue = analogRead(LDR_PIN) < 250;
      // Serial.println(analogRead(LDR_PIN));
      // digitalWrite(LED_BUILTIN, ldrValue);
      if (ldrValue > lastldrValue) {
        blinks++;
      }
      lastldrValue = ldrValue;
    }
    if (blinks <= 1) {
      //turn left
      forward(100, 50);
      turn_left(200, 130);
      stop();
      return;
    } else {
      //turn right
      forward(100, 50);
      turn_right(200, 130);
      stop();
    }

  //the code below handle moom laem
  } else if (sensorsReading[SensorName::RR] && sensorsReading[SensorName::C] && !sensorsReading[SensorName::R]) {
    forward(BASE_POWER, 700);
    while (!(sensorsReading[SensorName::RR] || sensorsReading[SensorName::R] || sensorsReading[SensorName::C])) {

      turn_right(HARD_TURN_POWER, 50);
      stop();
    }
    stop();
    directionLastMove = 1;
  } else if (sensorsReading[SensorName::LL] && sensorsReading[SensorName::C] && !sensorsReading[SensorName::L]) {
    forward(BASE_POWER, 700);
    while (!(sensorsReading[SensorName::LL] || sensorsReading[SensorName::L] || sensorsReading[SensorName::C])) {

      turn_left(HARD_TURN_POWER, 50);
      stop();
    }
    stop();
    directionLastMove = 1;
  }
  //not this code below nah
  else if (sensorsReading[SensorName::RR]) {
    lcd.fillRect(120, 50, 5, 5, GREEN);
    forward(BASE_POWER, 20);
    turn_right(HARD_TURN_POWER, 60);
    stop();
    lcd.fillRect(120, 50, 5, 5, WHITE);
    lastExtreme = SensorName::RR;
    directionLastMove = 1;
  } else if (sensorsReading[SensorName::LL]) {
    lcd.fillRect(10, 50, 5, 5, GREEN);
    forward(BASE_POWER, 20);
    turn_left(HARD_TURN_POWER, 60);
    stop();
    lcd.fillRect(10, 50, 5, 5, WHITE);
    lastExtreme = SensorName::LL;
    directionLastMove = 1;
  } else if (sensorsReading[SensorName::R]) {
    lcd.fillRect(30, 50, 5, 5, GREEN);
    forward(BASE_POWER, 20);
    turn_right(TURN_POWER, 60);
    stop();
    lcd.fillRect(20, 50, 5, 5, WHITE);
    directionLastMove = 1;
  } else if (sensorsReading[SensorName::L]) {
    lcd.fillRect(90, 50, 5, 5, GREEN);
    forward(BASE_POWER, 20);
    turn_left(TURN_POWER, 60);
    stop();
    lcd.fillRect(90, 50, 5, 5, WHITE);
    directionLastMove = 1;
  } else if (sensorsReading[SensorName::C]) {
    lcd.fillRect(60, 50, 5, 5, GREEN);
    forward(BASE_POWER, 60);
    stop();
    lcd.fillRect(60, 50, 5, 5, WHITE);
    directionLastMove = 1;
  } else {
    lcd.fillRect(60, 50, 5, 5, GREEN);
    forward(BASE_POWER, 60);
    stop();
    lcd.fillRect(60, 50, 5, 5, WHITE);
    directionLastMove = 0;
  }
  ldrBaseValue += analogRead(LDR_PIN);
  ldrBaseValue /= 2;
>>>>>>> Stashed changes
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
            lcd.fillRect(10 + 20 * i, 30, 5, 5, MAGENTA);
        } else {
            sensorsReading[i] = black;
            lcd.fillRect(10 + 20 * i, 30, 5, 5, WHITE);
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