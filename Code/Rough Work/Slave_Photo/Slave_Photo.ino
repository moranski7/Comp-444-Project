#include <Wire.h>

int photoresistor = 0;
char check =  ' ';

void setup() {
Wire.begin (9);
Wire.onReceive (receiveEvent);
Serial.begin (9600);
}

void receiveEvent (int bytes) {
  //photoresistor = Wire.read ();
  check = Wire.read ();
}

void loop() {
  Serial.print ("photoresistor: ");
  Serial.println (check);
}
