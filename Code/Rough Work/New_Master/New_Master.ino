#include <I2C_Anything.h>

long foo = 42;

void setup() {
  Wire.begin (); // Start the I2C Bus as Master
  Serial.begin(9600);
}

void loop() {
  Wire.beginTransmission (9);
  I2C_writeAnything (foo);
  if (Wire.endTransmission () == 0) {
    Serial.println ("Success!");
  }
  else {
    Serial.println ("Failure!");
  }
}
