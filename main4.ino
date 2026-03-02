#include "motor.h"

//Motors
const int motorRENPin = 41;
const int motorRIN2Pin = 39;
const int motorRIN1Pin = 37;

const int motorLENPin = 43;
const int motorLIN2Pin = 45;
const int motorLIN1Pin = 47;

Motor rightMotor(motorRIN1Pin, motorRIN2Pin, motorRENPin);
Motor leftMotor(motorLIN1Pin, motorLIN2Pin, motorLENPin);

const int motorRENPin2 = 31;
const int motorRIN2Pin2 = 35;
const int motorRIN1Pin2 = 33;

const int motorLENPin2 = 53;
const int motorLIN2Pin2 = 51;
const int motorLIN1Pin2 = 49;

Motor rightMotor2(motorRIN1Pin2, motorRIN2Pin2, motorRENPin2);
Motor leftMotor2(motorLIN1Pin2, motorLIN2Pin2, motorLENPin2);

const int driveSpeed = 150;
const int turnSpeed = 150; 				/// 235 turnspeed for 1500ms gives ~150 degrees rotation

// for search and orient
const int trigPin = 9;
const int echoPin = 10;
float duration, distance;
float array[100];
float array_min_val = 999999.0;
float direction, calbirated_val;
float locationData[100];

//edge detector pin
int frontDetectorPin = 2;


enum State {
  FORWARD,
  BACKWARD,
  TURN,
  AVOID,
  SEARCH,
};

State currentState = SEARCH;

void setup() {
  Serial.begin(9600);
}

void loop() {
  switch (currentState) {
    case FORWARD:             //move forward until reaching edge of arena
      moveForward();

      if (digitalRead(frontDetectorPin) == LOW) {
        currentState = AVOID;
      }
      break;
    

    case BACKWARD:              //currently only used when avoiding edge
      moveBackward();
      delay(2000);
      currentState = TURN;
      break;
		

		case TURN:              //currently only used when avoiding edge after having moved backward already
			turnLeft();
      delay(1500);
      currentState = FORWARD;
      break;
    

    case AVOID:
      currentState = BACKWARD;
      break;


    case SEARCH:
			locationData[100] = ultrasonicSearch();
      orient(locationData);
      currentState = FORWARD;
      break;

  }
}

/// MOTION FUNCTIONS
void turnLeft() {
    rightMotor.forward(turnSpeed);
    leftMotor.backward(turnSpeed);
    rightMotor2.forward(turnSpeed);
    leftMotor2.backward(turnSpeed);
}

void moveForward() {
    rightMotor.forward(driveSpeed);
    leftMotor.forward(driveSpeed);
    rightMotor2.forward(driveSpeed);
    leftMotor2.forward(driveSpeed);
}

void moveBackward() {
    rightMotor.backward(255);
    leftMotor.backward(255);
    rightMotor2.backward(255);
    leftMotor2.backward(255);
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
		return array[100];
}

void orient(float array[10]) {
  int i;
	for (i; i < 100; i++) {
		if (array[i] < array_min_val) {
			array_min_val = array[i];
			direction = i;
		}
	}
  Serial.println(i * 10);
	rightMotor.forward(turnSpeed);
  leftMotor.backward(turnSpeed);
  delay(i * 10);
}
