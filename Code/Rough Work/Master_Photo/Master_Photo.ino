#include <Wire.h>

int photoresistor = 0;

void setup() {
  Wire.begin (); // Start the I2C Bus as Master
  Serial.begin (9600);
}

void loop() {
  photoresistor = analogRead (A0);
  Serial.print ("Photoresistor: ");
  Serial.println (photoresistor);
  
  Wire.beginTransmission (9);   // transmitt to device #9
  Wire.write (photoresistor);
  Wire.endTransmission ();
  delay (500);
}
