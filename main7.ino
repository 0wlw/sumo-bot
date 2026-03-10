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

const int driveSpeed = 200;
const int turnSpeed = 200;

// ultrasonic
const int trigPin = 29;
const int echoPin = 27;

float duration, distance;
float dist;

int threshold = 80;

//edge detector pins
int frontDetectorPin = 2;
int backDetectorPin = 3;

bool SearchComplete = false;

unsigned long searchStartTime = 0;
const unsigned long searchTimeout = 8000;

enum State {
  FORWARD,
  BACKWARD,
  TURN,
  FRONTAVOID,
  BACKAVOID,
  SEARCH
};

State currentState = SEARCH;

void setup() {

  Serial.begin(9600);
  delay(5500);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(frontDetectorPin, INPUT);
  pinMode(backDetectorPin, INPUT);

  randomSeed(analogRead(A0));

  searchStartTime = millis();

  moveForward();
  delay(750);
}

void loop() {

  /// EDGE OVERRIDE (ALWAYS RUN FIRST)

  if (digitalRead(frontDetectorPin) == LOW) {

    Serial.println("FRONT EDGE DETECTED");

    brakeAll();
    delay(50);

    moveBackward();
    delay(600);

    brakeAll();
    delay(150);

    turnLeft();
    delay(600);

    brakeAll();

    SearchComplete = false;
    searchStartTime = millis();
    currentState = SEARCH;
    return;
  }

  if (digitalRead(backDetectorPin) == LOW) {

    Serial.println("BACK EDGE DETECTED");

    brakeAll();
    delay(50);

    moveForward();
    delay(600);

    brakeAll();
    delay(150);

    turnLeft();
    delay(600);

    brakeAll();

    SearchComplete = false;
    searchStartTime = millis();
    currentState = SEARCH;
    return;
  }

  find_distance();

  switch (currentState) {

    case FORWARD:

      moveForward();
      break;

    case BACKWARD:

      moveBackward();
      delay(200);

      SearchComplete = false;
      searchStartTime = millis();

      currentState = SEARCH;

      break;

    case TURN:

      turnLeft();
      delay(1500);

      SearchComplete = false;
      searchStartTime = millis();

      currentState = SEARCH;

      break;

    case SEARCH:

      Serial.println("Searching");

      ultrasonicSearch();

      if (SearchComplete == true) {

        moveForward();
        currentState = FORWARD;
        searchStartTime = millis();
      }

      if (millis() - searchStartTime > searchTimeout) {

        Serial.println("Search timeout - random reposition");

        brakeAll();
        delay(200);

        int randomTurnTime = random(300, 1500);

        turnLeft();
        delay(randomTurnTime);

        brakeAll();
        delay(200);

        moveForward();
        delay(350);

        brakeAll();

        searchStartTime = millis();
      }

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

void brakeAll() {

  rightMotor.brake();
  leftMotor.brake();

  rightMotor2.brake();
  leftMotor2.brake();
}


/// ULTRASONIC DISTANCE

float find_distance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = (duration * 0.0343) / 2;

  Serial.println(distance);

  return distance;
}


/// SEARCH FUNCTION

float ultrasonicSearch() {

  dist = find_distance();

  if (dist < threshold && dist > 0) {

    Serial.println("Target Found");

    SearchComplete = true;

    return dist;
  }

  leftMotor.forward(turnSpeed);
  leftMotor2.forward(turnSpeed);

  rightMotor.backward(turnSpeed);
  rightMotor2.backward(turnSpeed);

  delay(25);

  brakeAll();

  delay(100);

  return dist;
}
