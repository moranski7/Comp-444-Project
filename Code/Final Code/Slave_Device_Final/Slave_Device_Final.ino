/*
Author: Michael G. Oranski
Student ID: 2743708
Student Email: moranski1@athabasca.edu
Date: March 2, 2019
Note: Works only when communicating with another device that is running Master_Device_Final.ino
 */
#include <I2C_Anything.h>
#define TEST 4        // Used to index an array of function pointers. Swaps out the feedback control function. Range 0 to 4.
/*
 * 0  = proportional control
 * 1 = Integral Controller
 * 2 = Derivative Controller
 * 3 = Proportional-Derivative Controller
 * 4 = Proportional-integral-Derivative (PID) Controller
 */

//the left motor will be controlled by the motor B pins on the motor driver
const int PWMB = 8;           //speed control pin on the motor driver for the left motor
const int Bin2 = 9;           //control pin 2 on the motor driver for the left motor
const int Bin1 = 10;           //control pin 1 on the motor driver for the left motor

//the right motor will be controlled by the motor A pins on the motor driver
const int Ain1 = 11;           //control pin 1 on the motor driver for the right motor
const int Ain2 = 12;            //control pin 2 on the motor driver for the right motor
const int PWMA = 13;            //speed control pin on the motor driver for the right motor

volatile boolean haveData = false;      //Whatever device is getting data from master or not
volatile float sampleAudio;             // Sample of audio levels from master device
float preAduio = 0;                     // Previous Audio sample
float normal = -1;                      // Normal sound level
int onLight = 0;                        // whatever the light on the master device should be on or off.

// Control variables
// const float turnTheshold = 0.7;
const float turnTheshold = 0.45;   // min sound threshold for the robot to detect if it start to turn around
const float sourceThreshold = 3; // min sound threshold for the robot to detect if it think it found the source of the sound.
boolean turned = false;   // control variable. Determines if robot is turned towards sound.
boolean found = false;    //control variable. Determines if robot has fuond the source of the sonud.
float diff = 0.0;          // difference between sample and threshold.
float oldDiff = 1024.0;   // store the previous difference.

// Feedback control variables
float integral = 0.0;     // integral control variable
float lastError = 0.0;    // derivative control variable
const int kp = 1000;      // A feedback control variable for proportional
const int ki = 100;       // A feedback control variable for integrals
const int kd = 100;        // A feedback control variable for derivatives

//Feedback control functions

/*
 * Proportional Controller
 */
 float proportionalCon (float threshold) {
  Serial.println ("Proportional Controller");
  float error = threshold - sampleAudio; // Calculate the error variable
  float distance = kp*error;     // Calculate the control variable
  return distance;
}

/*
 *  Integral Controller
 */
float integralCon (float threshold) {
  Serial.println ("Integral Controller");
  float error = threshold - sampleAudio;  // Calculate the error
  integral = integral + error;            // Calculate the integral
  float distance = integral * ki;         // Calculate the Control Variable
  return distance; 
}

/*
 * Derivative Controller
 */
 float derivativeCon (float threshold) {
  Serial.println ("Derivative Controller");
  float error = threshold - sampleAudio; // Calculate the error
  float derivative = abs(error - lastError);   // Calculate the derivative
  float distance = (kd * derivative);     // Calculate the control Variable
  lastError = error;                      // Save the current error as last error for next iteration
  return distance;
 }

 /*
  * Proportional-Derivative Controller
  */
float pdCon (float threshold) {
  Serial.println ("Proportional-Derivative Controller");
  float error = threshold - sampleAudio;                // Calculate the error
  float derivative = error - lastError;                // Calculate the derivative
  float distance = (kp*error) + (kd * derivative);     // Calculate the control Variable
  lastError = error;                                   // Save the current error as last error for next iteration
  return distance;
}

/*
 * Proportional-integral-Derivative (PID) Controller
 */
float pidCon (float threshold) {
  Serial.println ("PID Controller");
  float error = threshold - sampleAudio;                // Calculate the error
  float derivative = error - lastError;                // Calculate the derivative
  integral = integral + error;                         // Calculate the integral
  
  float distance = (kp*error) + (kd * derivative) + (integral * ki);     // Calculate the control Variable
  lastError = error;                                                    // Save the current error as last error for next iteration
  return distance;
}

// Stores the feeback control functions in an array for easier access
typedef float (*FB) (float);
FB funcs[] = {proportionalCon, integralCon, derivativeCon, pdCon, pidCon};

void setup() {

  // Setup the pins for right motor
  pinMode (Ain1, OUTPUT);
  pinMode (Ain2, OUTPUT);
  pinMode (PWMA, OUTPUT);

  // Setup the pins for the left motor
  pinMode (Bin1, OUTPUT);
  pinMode (Bin2, OUTPUT);
  pinMode (PWMA, OUTPUT);
  
  Wire.begin (9); // Set the wire to address nine
  // Registers the events
  Wire.onReceive (receiveEvent);
  Wire.onRequest (requestEvent);
  
  Serial.begin (9600); // begin serial connection for debugging
}

void receiveEvent (int bytes) {
  if (bytes >= (sizeof sampleAudio)) { // Checks to see if enough data is available
    I2C_readAnything (sampleAudio);   // Gets data from wire
    haveData = true;
  }
} // end of receiveEvent

/*
 * Function for slave sending message to master device
 */
void requestEvent () {
  unsigned int sizeVar = sizeof (onLight);
  Wire.write ((byte *) &onLight, sizeVar);
}  // end of requestEvent

void loop() {
  //forward (200, 250);
  //right (200, 250);
  //left (200, 250);
  //backward (200, 250);
  if ((haveData == true) && (found == false)) { // Checks for a successful data transfer
    Serial.println ("Receive Success!");
    Serial.println (sampleAudio);

    // Callibrate the robot to a normal environment
    if (normal == -1) {
      normal = sampleAudio;
    }

    findSource (); // Spin robot around
    zeroIn (); // Move the robot towards the source
 
  }
  else {
    Serial.println ("Receive Failure!");
  }

  // Set to one to inform master device that source has been found. 
  if (found == true) {
    onLight = 1;
  }
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
  if (((normal + 0.0) < sampleAudio) && (turned == false)) { // If detected large sound and the robot hasn't turned.
    Serial.println ("NOISE DETECTED!");
    Serial.print ("sampleAudio: ");
    Serial.println (sampleAudio);
    Serial.print ("preAduio: ");
    Serial.println (preAduio);

    diff = turnTheshold - sampleAudio;
    if (diff < oldDiff) {
      oldDiff = diff;
    }

    float distance = funcs[TEST] (turnTheshold);

    if (sampleAudio >= turnTheshold) {
      Serial.println ("Sound Direction found!");
      turned = true;
    }
    // If the sample Audio is less than the threshold
    else if ((sampleAudio < turnTheshold) && (diff <= oldDiff)){
      Serial.println ("Turning right towards Sound!");
      right (255, distance);
      delay (100);
    }
    else if ((sampleAudio < turnTheshold) && (diff > oldDiff)) {
      Serial.println ("Turning left towards Sound!");
      left (255, distance);
      delay (100);
    }
  }
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

    float distance = funcs[TEST] (sourceThreshold);

    // No object but source is far away
    if (sampleAudio < sourceThreshold) {
      Serial.println ("Moving Forward.");
      forward (255, distance);
    }
    // Found source of noise
    else if (sampleAudio >= sourceThreshold) {
      Serial.println ("Found Source!");
      found = true;
    }
  }
  delay (100);
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
