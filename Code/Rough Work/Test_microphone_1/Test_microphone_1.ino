// Works!

const int sampleWindow  = 1; // Sample window width in mS (1 mS = 1000 Hz)
unsigned int sample;

void setup() {
  Serial.begin (9600);
}

void loop() {
  unsigned long startMillis = millis (); // Start of sample window
  unsigned int peakToPeak = 0;  // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  Serial.println ("Next set of samples");

  while (millis () - startMillis < sampleWindow) {
    sample = analogRead (0);

    if (sample < 1024) {

      if (sample > signalMax) {
        signalMax = sample; // save just the max level
      }
      if (sample < signalMin)
      {
        signalMin = sample; // save just the min level
      }
    }
    Serial.print ("Sample is: ");
    Serial.println (sample);
  }
  peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude
  double volts = (peakToPeak * 5.0) / 1024; //  convert to volts

  Serial.print ("SampleMin: ");
  Serial.print (signalMin);
  Serial.print ("SampleMax: ");
  Serial.println (signalMax);
  Serial.print ("Volts is: ");
  Serial.println (volts);
  Serial.println ();
  delay (200);
}
