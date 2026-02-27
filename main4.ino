#include "motor.h"

const int motorRENPin = 10;
const int motorRIN2Pin = 9;
const int motorRIN1Pin = 8;

const int motorLENPin = 5;
const int motorLIN2Pin = 7;
const int motorLIN1Pin = 6;

const int driveSpeed = 235;
const int turnSpeed = 235; 				/// 235 turnspeed for 1500ms gives ~150 degrees rotation

const int trigPin = 9;
const int echoPin = 10;

float duration, distance;
float array[10];
float array_min_val = 999999.0;
float direction, calbirated_val;

Motor rightMotor(motorRIN1Pin, motorRIN2Pin, motorRENPin);
Motor leftMotor(motorLIN1Pin, motorLIN2Pin, motorLENPin);

int frontDetectorPin = 2;
//int backDetectorPin = 3;

enum State {
  FORWARD,
  BACKWARD,
  TURN,
  AVOID,
  SEARCH,
};

State currentState = FORWARD;

void setup() {
  Serial.begin(9600);
}

void loop() {
  switch (currentState) {
    case FORWARD:
      moveForward()

      if (digitalRead(frontDetectorPin) == LOW) {
        currentState = AVOID;
      }
      break;
    

    case BACKWARD:
      moveBackward();
      delay(2000);
      currentState = TURN;
		

		case TURN:
			turnLeft();
    

    case AVOID:
      moveBackward()
      delay(3000);
      turnLeft()
      delay(1500);
      currentState = FORWARD;
      break;


    case SEARCH:
			ultrasonicSearch()
  }
}

/// MOTION FUNCTIONS
void turnLeft() {
    rightMotor.forward(turnSpeed);
    leftMotor.backward(turnSpeed);
}

void moveForward() {
    rightMotor.forward(driveSpeed);
    leftMotor.forward(driveSpeed);
}

void moveBackward() {
    rightMotor.backward(driveSpeed);
    leftMotor.backward(driveSpeed);
}


//SEARCH AND ORIENTATION FUNCTIONS
float find_distance() { 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  return distance;
}

float ultrasonicSearch() {
    for (int i; i < 100; i++) {
      array[i] == find_distance();							///0.1 deg / ms
      rightMotor.forward(driveSpeed);
      leftMotor.backward(driveSpeed);
      delay(36);
    }
		return array;
}

float determineOrientation() {
	for (int i; i < 10; i++) {
		if (array[i] < array_min_val) {
			array_min_val = array[i];
			direction = i;
		}
	}
	rightMotor.forward()
}
