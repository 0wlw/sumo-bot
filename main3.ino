#include "motor.h"

const int motorRENPin = 10;
const int motorRIN2Pin = 9;
const int motorRIN1Pin = 8;

const int motorLENPin = 5;
const int motorLIN2Pin = 7;
const int motorLIN1Pin = 6;

const int driveSpeed = 235;
const int turnSpeed = 235;

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
  DRIVE,
  AVOIDFRONT,
  SEARCH,
};

State currentState = DRIVE;

void setup() {
  Serial.begin(9600);
}

void loop() {
  switch (currentState) {
    case DRIVE:
      rightMotor.forward(driveSpeed);
      leftMotor.forward(driveSpeed);
      if (digitalRead(frontDetectorPin) == LOW) {
        currentState = AVOIDFRONT;
      }
      break;
    case AVOIDFRONT:
      rightMotor.backward(driveSpeed);
      leftMotor.backward(driveSpeed);
      delay(3000);
      rightMotor.forward(driveSpeed);
      leftMotor.backward(driveSpeed);
      delay(1500);
      currentState = DRIVE;
      break;

    case SEARCH:
      for (int i; i < 10; i++) {
        array[i] == find_distance();
        rightMotor.forward(driveSpeed);
        leftMotor.backward(driveSpeed);
        delay(25);
      }
      for (int i; i < 10; i++) {
        if (array[i] < array_min_val) {
          array_min_val = array[i];
          direction = i * calibrated_val;
        }
      }

  }
}

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
