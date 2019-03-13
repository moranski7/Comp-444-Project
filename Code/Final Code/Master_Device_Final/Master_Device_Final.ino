/*
Author: Michael G. Oranski
Student ID: 2743708
Student Email: moranski1@athabasca.edu
Date: March 2, 2019
Note: Works only when communicating with another device that is running Slave_Device_Final.ino
 */
#include <I2C_Anything.h>

const int MIC = A1;       // sound sensor's pin
const int AMP = A0;       // Amp sensor's pin
const int BLUELED = 13;   // BLUE LED pin
const int REDLED = 12;    // RED LED pin
int switchPin = 7;        //switch to turn the robot on and off


// Sample variables
float sampleAudio = 0;    // sample of audio level in environment
int onLight = 0;          // light on the master circuit should be on or off
const int sampleWindow  = 1; // Sample window width in mS (1 mS = 1000 Hz)
unsigned int sampleAmp;      // Sample of the frequency of audio


void setup() {
  pinMode (BLUELED, OUTPUT);  // Set up BLUE LED pin for output
  pinMode (REDLED, OUTPUT);   // Set up Red Led pin for output
  pinMode(switchPin, INPUT_PULLUP);   //set this as a pullup to sense whether the switch is flipped

  Wire.begin ();         // Start the I2C Bus as Master
  Serial.begin (9600);  // Begin serial connection for debugging purpose
}

void loop() {
  sampleAudio = takeSample ();        // Takes the average sample from sensor.
  Serial.print ("sampleAudio: ");
  Serial.println (sampleAudio);

  if(digitalRead(switchPin) == LOW){  //if the on switch is flipped
    Wire.beginTransmission (9);         // Begin wire transfer to device
    I2C_writeAnything (sampleAudio);    // Transfer over sample to slave device
    if (Wire.endTransmission() == 0) {  // Checks to see if transfer is success
      Serial.println ("Transfer Success!");
    }
    else {
      Serial.println ("Transfer Failure!");
    }
  }

  Serial.println ("Requesting data!");
  Wire.requestFrom (9,2); // request 2 bytes from slave device #9

  // Checks to see if there is any data from the slave device
  while (Wire.available ()) {
    Serial.println ("Received data!");
    onLight = Wire.read (); // Retrieves the data from slave and stores
    Serial.print ("onLight: ");
    Serial.println (onLight);

    if (onLight == 1) {
      double volts = GetAmp();

      if (volts >= 1) {
        digitalWrite (REDLED, HIGH);
        delay (200);
      }
      else {
        digitalWrite (BLUELED, HIGH);
        delay (200);
      }
    } // end of outer if-statement

    digitalWrite (REDLED, LOW);
    digitalWrite (BLUELED,LOW);
  } // end of while statement
}

/*
 * Takes a couple of samples from audio and averages them out. 
 * Reduces the number of sudden spikes in audio levels. 
 * Param:
 *  N/A
 * Return:
 *  A float that represents the average of five audio samples.
 */
float takeSample () {
  float sumSample = 0;

  // sum five audio samples together
  for (int i = 0; i < 5; i++) {
    float sample = (float) analogRead (MIC);
    sumSample += sample;
    delay (10);
  }
  return sumSample / 5; // divides the samples by five and return. 
}

double GetAmp() {
  unsigned long startMillis = millis (); // Start of sample window
  unsigned int peakToPeak = 0;  // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  Serial.println ("Next set of samples");

  while (millis () - startMillis < sampleWindow) {
    sampleAmp = analogRead (AMP);

    if (sampleAmp < 1024) {

      if (sampleAmp > signalMax) {
        signalMax = sampleAmp; // save just the max level
      }
      if (sampleAmp < signalMin)
      {
        signalMin = sampleAmp; // save just the min level
      }
    }
    Serial.print ("Sample is: ");
    Serial.println (sampleAmp);
  }
  peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude
  double volts = (peakToPeak * 5.0) / 1024; //  convert to volts
  
  return volts;
}

