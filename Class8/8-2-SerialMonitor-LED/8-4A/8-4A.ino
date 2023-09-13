#include <SoftwareSerial.h>

SoftwareSerial mySerial(10,11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available())
  {
    mySerial.print((char)Serial.read());
    Serial.print("ME:");
    Serial.print((char)Serial.read());
    Serial.println("");
  }
  while (mySerial.available()) 
  {
    Serial.print("B:");
    Serial.print((char)mySerial.read());
    Serial.println("");
  }
}
