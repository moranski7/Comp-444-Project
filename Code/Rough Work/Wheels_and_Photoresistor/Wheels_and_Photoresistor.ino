// THIS WORKS!

int photoresistor = 0;
const int light = 850;
int normal = -1;

const int PWMB = 10;
const int Bin2 = 13;
const int Bin1 = 12;

const int Ain1 = 8;
const int Ain2 = 9;
const int PWMA = 11;

const int LED = 7;

void setup() {
  pinMode (Ain1, OUTPUT);
  pinMode (Ain2, OUTPUT);
  pinMode (PWMA, OUTPUT);

  pinMode (Bin1, OUTPUT);
  pinMode (Bin2, OUTPUT);
  pinMode (PWMA, OUTPUT);

  pinMode (LED, OUTPUT);
  digitalWrite (LED, LOW);
  
  Serial.begin (9600);
  Serial.println ("Let begin the testing.");
}

void loop() {
    photoresistor = analogRead (A0);
    Serial.print ("Light level: ");
    Serial.println (photoresistor);
    delay (100);
    
    if (normal == -1) {
      normal = photoresistor;
    }

    if (((normal + 100) < photoresistor))
    {
      Serial.println ("Light detected!");
      Serial.print ("photoresistor: ");
      Serial.println (photoresistor);

      Serial.print ("light: ");
      Serial.println (light);
      if (photoresistor < light) {
        Serial.println ("Moving towards light!");
        rightMotor (200);
        leftMotor (-200);
        delay (250);
        rightMotor (0);
        leftMotor (0);
      }
      else {
      Serial.println ("Light found!");
      digitalWrite (LED, HIGH);
      delay (100);
    }
      /* This works! Adjust speed too.
       * rightMotor (255);
      leftMotor (200);
      delay (100);
      rightMotor (0);
      leftMotor (0); */
      
    }
    digitalWrite (LED, LOW);
}

void rightMotor (int motorSpeed) {
  if (motorSpeed > 0) {
    digitalWrite (Ain1, HIGH);
    digitalWrite (Ain2, LOW);
  }
  else if (motorSpeed < 0) {
    digitalWrite (Ain1, LOW);
    digitalWrite (Ain2, HIGH);
  }
  else {
    digitalWrite (Ain1, LOW);
    digitalWrite (Ain2, LOW);
  }
  analogWrite (PWMA, abs(motorSpeed));
}

void leftMotor (int motorSpeed) {
  if (motorSpeed > 0) {
    digitalWrite (Bin1, HIGH);
    digitalWrite (Bin2, LOW);
  }
  else if (motorSpeed < 0) {
    Serial.println ("Test");
  digitalWrite (Bin1, LOW);
  digitalWrite (Bin2, HIGH);
  }
  else {
    digitalWrite (Bin1, LOW);
    digitalWrite (Bin2, LOW);
  }

  analogWrite (PWMB, abs (motorSpeed));
}
