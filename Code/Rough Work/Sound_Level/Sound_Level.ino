// Data collecting

const int micPin = A1;
unsigned int ampSample;
float micValue = 0;

const int sampleWindow  = 1; // Sample window width in mS (1 mS = 1000 Hz)

void setup() {
Serial.begin (9600);
}

void loop() {
  //micValue = analogRead (A1);
  float sample = takeSample ();

  unsigned long startMillis = millis (); // Start of the sample window
  unsigned long peakToPeak = 0; // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  while (millis () - startMillis < sampleWindow) {
    ampSample = analogRead (0);
    
    if (sample < 1024) {

      if (ampSample > signalMax) {
        signalMax = ampSample; // save just the max level
      }
      if (ampSample < signalMin)
      {
        signalMin = ampSample; // save just the min level
      }
    }
  }

  peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude
  double volts = (peakToPeak * 5.0) / 1024; //  convert to volts
  
  //Serial.print ("micValue: ");
  //Serial.println (micValue);
  Serial.print ("sample: ");
  Serial.println (sample);
  Serial.print ("Volts is: ");
  Serial.println (volts);
  Serial.println ();
  delay (300); 
}

float takeSample () {
  float sumSample = 0;
  for (int i = 0; i < 5; i++) {
    float sample = (float) analogRead (A1);
    //Serial.print ("takeSample ()/sample: ");
    //Serial.println (sample);
    sumSample += sample;
    delay (10);
  }
  return sumSample / 5;
}

