// THIS WORKS!

int photoresistor = 0;
const int light = 850;
int normal = -1;
int last = 0;
int trig = 0;
long startTime = 0; // timer variable for time limit on button press
long limit = 500;  // time limit to hit a button

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
    /*forward (255, 300);
    delay (1000); */
    startTime = millis ();   // records the start time
    photoresistor = analogRead (A0);
    Serial.print ("Photoresistor: ");
    Serial.println (photoresistor);
    delay (100);
    
    if (normal == -1) {
      normal = photoresistor;
    }

    // Spin around
    findSource ();
    // Move towards the center
    zeroIn ();

    if (trig == 1) {
      digitalWrite (LED, HIGH);
      delay (400);
    }
    else if (millis () - startTime > limit) { // if the time limit is up 
      normal = -1;
      last = 0;
    }
    digitalWrite (LED, LOW);
    delay (100);
}

void forward (int motorSpeed, int distance) {
  rightMotor (motorSpeed);
  leftMotor (motorSpeed);
  delay (distance);
  rightMotor (0);
  leftMotor (0);
}

void backward (int motorSpeed, int distance) {
  rightMotor (-motorSpeed);
  leftMotor (-motorSpeed);
  delay (distance);
  rightMotor (0);
  leftMotor (0);
}

void right (int motorSpeed, int distance) {
  rightMotor (motorSpeed);
  leftMotor (-motorSpeed);
  delay (distance);
  rightMotor (0);
  leftMotor (0);
}

void left (int motorSpeed, int distance) {
  rightMotor (-motorSpeed);
  leftMotor (motorSpeed);
  delay (distance);
  rightMotor (0);
  leftMotor (0);
}

void findSource () {
  if (((normal + 100) < photoresistor) && last == 0)
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
        delay (200);
        Serial.println ("findSource(): recording last");
        last = photoresistor;
        Serial.print ("Last: ");
        Serial.println (last);
      } 
    }
    digitalWrite (LED, LOW);
}

void adjust () {
  if ((last > 0) && (trig == 0)) {
    right (150, 150);
    photoresistor = analogRead (A0);
    
    if ((last) > (photoresistor)) {
      trig = 1;
      left (150, 150);
    }
    else
    {
      Serial.println ("Adjust(): recording last.");
      last = photoresistor;
      Serial.print ("Last: ");
      Serial.println (last);
    }
  }
}

void zeroIn () {
  if ((last > 0) && (trig == 0)) {
    Serial.println ("Moving forward");

    if (photoresistor < 950) {
      forward (255, 150);
    }
    else if (photoresistor >= 950) {
      trig = 1;
    }
  }
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
