#include <I2C_Anything.h>

int photoresistor = 0;
const int LED = 13;
int onLight = 0;

void setup() {
  pinMode (LED, OUTPUT);
  Wire.begin (); // Start the I2C Bus as Master
  Serial.begin(9600);
}

void loop() {
  photoresistor = analogRead (A0);
  
  Wire.beginTransmission (9);
  I2C_writeAnything (photoresistor);
  if (Wire.endTransmission () == 0) {
    Serial.println ("Success!");
    Serial.println (photoresistor);
  }
  else {
    Serial.println ("Failure!");
  }

  Serial.println ("Requesting data!");
  Wire.requestFrom (9,2); // request 2 bytes from slave device #9

  while (Wire.available()) {
    Serial.println ("Received data!");
    onLight = Wire.read();
    Serial.print ("onLight: ");
    Serial.println (onLight);
    
    if (onLight == 1) {
      Serial.println ("Lights on!");
      digitalWrite (LED, HIGH);}
    if (onLight == 0) {
      Serial.println ("Lights off!");
      digitalWrite (LED, LOW);
    }
    delay (200);
  }
}
