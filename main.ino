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


void setup() {
  Serial.begin(9600);
  delay(5000);
}

void loop() {
  avoid_edge();
  if (digitalRead(2) < 1) {
    currentState = CHARGE;
    attack();
  }


  
}

void avoid_edge() {
  LineSensorFront = digitalRead(4);
  LineSensorBack = digitalRead(3);
  
  if (LineSensorFront < 1) {
    rightMotor.backward(driveSpeed - 25);
    leftMotor.backward(driveSpeed);
    delay(1000);
    rightMotor.forward(driveSpeed - 25);
    leftMotor.backward(driveSpeed);
    delay(100);
  } else if (LineSensorFront < 1) {
    rightMotor.forward(driveSpeed - 25);
    leftMotor.forward(driveSpeed);
    delay(1000);
    rightMotor.forward(driveSpeed - 25);
    leftMotor.backward(driveSpeed);
    delay(100);
  } else {
    DriveAround();
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
