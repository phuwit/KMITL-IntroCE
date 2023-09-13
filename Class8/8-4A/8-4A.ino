#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    if (Serial.available() > 0) {
        Serial.print("ME:");
        while (Serial.available()) {
            char recievedChar = (char)Serial.read();
            mySerial.print(recievedChar);
            Serial.print(recievedChar);
        }
    Serial.println("");
    }
    

    if (mySerial.available() > 0) {
        Serial.print("B:");
        while (mySerial.available()) {
            Serial.print((char)mySerial.read());
            delay(20);
        }
        Serial.println("");
    }
}
