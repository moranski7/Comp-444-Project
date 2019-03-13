int photoresistor = 0;
const int threshold = 850;

void setup() {
  Serial.begin (9600);
  Serial.println ("Let begin the testing.");
}

void loop() {
    photoresistor = analogRead (A0);
    Serial.print ("Light level: ");
    Serial.println (photoresistor);
    delay (100);
}
