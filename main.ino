#include "motor.h"

const int motorRENPin = 10;
const int motorRIN2Pin = 9;
const int motorRIN1Pin = 8;

const int motorLENPin = 5;
const int motorLIN2Pin = 7;
const int motorLIN1Pin = 6;

const int driveSpeed = 255;
const int turnSpeed = 255;

Motor rightMotor(motorRIN1Pin, motorRIN2Pin, motorRENPin);
Motor leftMotor(motorLIN1Pin, motorLIN2Pin, motorLENPin);

int LineSensorFront, LineSensorBack;
int attackDriveSpeed;

enum State {
  IDLE,
  SEARCH,
  PUSH,
  AVOIDEDGE,
  CHARGE,
  END
};

State currentState = IDLE;

void avoid_edge_front() {
  rightMotor.backward(driveSpeed - 25);
  leftMotor.backward(driveSpeed);
  delay(1000);
  rightMotor.forward(driveSpeed - 25);
  leftMotor.backward(driveSpeed);
  delay(100);
}

void avoid_edge_back() {
  rightMotor.forward(driveSpeed - 25);
  leftMotor.forward(driveSpeed);
  delay(1000);
  rightMotor.forward(driveSpeed - 25);
  leftMotor.backward(driveSpeed);
  delay(100);
}

void setup() {
  Serial.begin(9600);
  delay(5000);

  attachInterrupt(digitalPinToInterrupt(4, avoid_edge_front, FALLING));
  attachInterrupt(digitalPinToInterrupt(3, avoid_edge_back, FALLING));
}

void loop() {
  switch (currentState) {
    case IDLE:
      delay(5000);
      currentState = SEARCH;
    
    case SEARCH
  }
}

void DriveAround() {
  rightMotor.forward(driveSpeed - 25);
  leftMotor.forward(driveSpeed);
}

void CHECK_IF_END() {
  if (millis() > 61000) {
    currentState = END;
  }
}

void attack() {
  attackDriveSpeed = 255;
  rightMotor.forward(attackDriveSpeed);
  leftMotor.forward(attackDriveSpeed);
  delay(1000);
}
