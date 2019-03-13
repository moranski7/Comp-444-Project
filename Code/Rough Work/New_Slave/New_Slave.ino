#include <I2C_Anything.h>

volatile boolean haveData = false;
volatile long foo;
volatile  float bar;

void setup() {
  Wire.begin (9);
  Wire.onReceive (receiveEvent);
  Serial.begin (9600);
}

void receiveEvent (int bytes) {
  if (bytes >= (sizeof foo)) {
    I2C_readAnything (foo);
    haveData = true;
  } // end if have enough data
} // end of receiveEvent

void loop() {
  if (haveData == true) {
    Serial.println ("Success!");
    Serial.println (foo);
  }
  else {
    Serial.println ("Failure!");
  }
}
