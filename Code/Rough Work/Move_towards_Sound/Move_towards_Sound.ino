// This works!

// Pin variables
const int LED = 2;     // Pin  for LED light
const int TRIG = 6;    // Pin for the trig pin of the ultrasonic
const int ECHO = 5;    // Pin for the echo pin of the ultrasonic
const int MIC = A0;    // Pin for Sound Sensor

// Pins for Left Motor
const int PWMB = 10;
const int Bin2 = 13;
const int Bin1 = 12;

// Pins for Right Motor
const int Ain1 = 8;
const int Ain2 = 9;
const int PWMA = 11;

// Sample variables
float sampleAudio = 0;    // sample of audio level in environment
float preAduio = 0;       // Previous Audio sample
float normal = -1;        // Normal sound level

// Control variables
const float turnTheshold = 3.0;   // min sound threshold for the robot to detect if it start to turn around
const float sourceThreshold = 22.5; // min sound threshold for the robot to detect if it think it found the source of the sound.
boolean turned = false;   // control variable. Determines if robot is turned towards sound.
boolean found = false;    //control variable. Determines if robot has fuond the source of the sonud.

void setup() {
  Serial.begin (9600); // Start a serial connection
  
  // Set up pins for motor
  pinMode (Ain1, OUTPUT);
  pinMode (Ain2, OUTPUT);
  pinMode (PWMA, OUTPUT);

  pinMode (Bin1, OUTPUT);
  pinMode (Bin2, OUTPUT);
  pinMode (PWMA, OUTPUT);

  // set up pin for LED
  pinMode (LED, OUTPUT);
  digitalWrite (LED, LOW);
}

void loop() {
  //forward (200, 250);
  //delay (500);

  if (found == false) {
    preAduio = sampleAudio;
    sampleAudio = takeSample ();

    Serial.print ("sampleAudio: ");
    Serial.println (sampleAudio);
    Serial.print ("preAudio: ");
    Serial.println (preAduio);
    Serial.print ("normal: ");
    Serial.println (normal);
  }

  // Callibrate the robot to a normal environment
  if (normal == -1) {
    normal = sampleAudio + 1;
  }
  findSource (); // Spin robot around
  zeroIn (); // Move robot towards the sound

  if (turned == true) {
    digitalWrite (LED, HIGH);
    delay (200);
  }
  digitalWrite (LED, LOW);
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

/*
 * Turns the robot to the direction the sound is coming from.
 * 
 * Param:
 *    N/A
 *    
 * Return:
 *    N/A
 */
void findSource () {
  if (((normal + 0.5) < sampleAudio) && (turned == false)) { // If detected large sound and the robot hasn't turned.
    Serial.println ("NOISE DETECTED!");
    Serial.print ("sampleAudio: ");
    Serial.println (sampleAudio);
    Serial.print ("preAduio: ");
    Serial.println (preAduio);

    // If the sample Audio is less than the threshold
    if (sampleAudio < turnTheshold){
      Serial.println ("Turning towards Sound!");
      right (200, 250);
      delay (100);
    }
    // If the sample Adudio is greater to threshold but the previous audio is less than threshold 
    else if ((sampleAudio >= turnTheshold) && (preAduio < turnTheshold)) {
      Serial.println ("Turning towards Sound!");
      right (200, 250);
      delay (100);
    }
    // If the sample audio and the previous audio are both greater than threshold
    else if ((sampleAudio >= turnTheshold) && (preAduio >= turnTheshold)) {
      Serial.println ("Sound Direction found!");
      digitalWrite (LED, HIGH);
      delay (200);
      turned = true;
    }
  }
  digitalWrite (LED,LOW); // turn off LED
}

/*
 * Moves the robot towards the source of the sound while avoiding objects in the way.
 * 
 * Param:
 *    N/A
 * Return:
 *  N/A
 */
void zeroIn () {
  if ((turned == true) && (found == false)) {

    // Do object detection stuff here!
    
    if (sampleAudio < sourceThreshold) {
      Serial.println ("Moving Forward.");
      forward (255, 150);
    }
    else if (sampleAudio >= sourceThreshold) {
      Serial.println ("Found Source!");
      found = true;
    }
  }
} 

/*
 * Makes the robot moves backward from the direction it is facing.
 * Param:
 *  motorSpeed    - Controls the speed of the motors
 *  distance      - Controls how far the robot moves in the direction
 * Return:
 *  N/A
 */
void backward (int motorSpeed, int distance) {
  rightMotor (-motorSpeed); // Activate the right motor. negative for reverse
  leftMotor (-motorSpeed); // Activate the left motor. negative for reverse
  delay (distance); // Waits for the robot to move to the desired length
  rightMotor (0); // Stop the right motor
  leftMotor (0); // Stop the left motor
}

/*
 * Makes the robot moves to the right from the direction it is facing.
 * Param:
 *  motorSpeed    - Controls the speed of the motors
 *  distance      - Controls how far the robot moves in the direction
 * Return:
 *  N/A
 */
void right (int motorSpeed, int distance) {
  rightMotor (motorSpeed); // Right motor moves forward
  leftMotor (-motorSpeed); // Left motor moves backward. Negative = reverse
  delay (distance); // wait for the robot to move to the desired length
  rightMotor (0); // stop right motor
  leftMotor (0); // stop left motor
}

/*
 * Makes the robot moves forward in the direction it is facing.
 * Param:
 *  motorSpeed    - Controls the speed of the motors
 *  distance      - Controls how far the robot moves in the direction
 * Return:
 *  N/A
 */
void forward (int motorSpeed, int distance) {
  rightMotor (motorSpeed); // Active the right motor
  leftMotor (motorSpeed); // Active the left motor
  delay (distance);       // Waits for the robot to move to the desired direction
  rightMotor (0);         // Stop the right motor
  leftMotor (0);          // Stop the left motor
}

/*
 * Makes the robot moves to the left from the direction it is facing.
 * Param:
 *  motorSpeed    - Controls the speed of the motors
 *  distance      - Controls how far the robot moves in the direction
 * Return:
 *  N/A
 */
void left (int motorSpeed, int distance) {
  rightMotor (-motorSpeed); // right motor moves backward. Negative = reverse
  leftMotor (motorSpeed); // left motor moves forward.
  delay (distance); // Waits for the robot to move to the desired direction
  rightMotor (0); // stops the right motor
  leftMotor (0); // stops the left motor
}

/*
 * Controls the motor speed of the right motor.
 * Param:
 *  motorSpeed    - Controls the speed of the motor. Can't Go lower than 150 or engine won't work.
 *  Return:
 *  N/A
 */
void rightMotor (int motorSpeed) {
  // if motorSpeed is positive, move Clockwise
  if (motorSpeed > 0) {
    digitalWrite (Ain1, HIGH);
    digitalWrite (Ain2, LOW);
  }
  // If motorSpeed is negative, move CounterClockwise
  else if (motorSpeed < 0) {
    digitalWrite (Ain1, LOW);
    digitalWrite (Ain2, HIGH);
  }
  // If zero, stop motor
  else {
    digitalWrite (Ain1, LOW);
    digitalWrite (Ain2, LOW);
  }
  // Controls the speed of the motor.
  analogWrite (PWMA, abs(motorSpeed));
}

/*
 * Controls the motor speed of the left motor.
 * Param:
 *  motorSpeed    - Controls the speed of the motor. Can't Go lower than 150 or engine won't work.
 *  Return:
 *  N/A
 */
void leftMotor (int motorSpeed) {
  // if motorSpeed is positive, move Clockwise
  if (motorSpeed > 0) {
    digitalWrite (Bin1, HIGH);
    digitalWrite (Bin2, LOW);
  }
  // If motorSpeed is negative, move CounterClockwise
  else if (motorSpeed < 0) {
    digitalWrite (Bin1, LOW);
    digitalWrite (Bin2, HIGH);
  }
  // If zero, stop motor
  else {
    digitalWrite (Bin1, LOW);
    digitalWrite (Bin2, LOW);
  }
  // Controls the speed of the motor.
  analogWrite (PWMB, abs (motorSpeed));
}

/*
 * Sends out a sound pulse and times how long it takes to come back.
 * Based on the time, detects distances from objects.
 * 
 * Param:
 *    N/A
 * Return:
 *    A float that represents the distance from an object in front of the robot.
 */
float getDistance () {
  float echoTime;   // variable to store the time it takes for a ping to bounce off an object
  float calcualtedDistance;   // variable to store the distance calculated from the echo time

  // send out an ultrasonic pulse that's 10ms long
  digitalWrite (TRIG, HIGH);
  delayMicroseconds (10);
  digitalWrite (TRIG, LOW);

  echoTime = pulseIn (ECHO, HIGH); // use the pulseIn command to see how long it takes for the pulse to bounce back

  Serial.print ("echoTime: ");
  Serial.println (echoTime);
  calcualtedDistance = echoTime / 148.0;  // calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)
  return calcualtedDistance;              // send back the distance that was calculated
}
