#include "Controller.h"

Controller::Controller() {

}

Controller::~Controller() {

}

bool Controller::begin() {
  pinMode(PETSR2_LEVER_L, OUTPUT);
  pinMode(PETSR2_LEVER_R, OUTPUT);
  pinMode(PETSR2_POWER, OUTPUT);

  stopMove();
  
  return true;
}

void Controller::forward() {
  // Serial.println("forward");
  digitalWrite(PETSR2_POWER, HIGH);

  digitalWrite(PETSR2_LEVER_L, LOW);
  digitalWrite(PETSR2_LEVER_R, LOW);
}

void Controller::backward() {
  // Serial.println("backward");
  digitalWrite(PETSR2_POWER, HIGH);

  digitalWrite(PETSR2_LEVER_L, HIGH);
  digitalWrite(PETSR2_LEVER_R, HIGH);
}

void Controller::stopMove() {
  // Serial.println("stopMove");
  digitalWrite(PETSR2_POWER, LOW);

  digitalWrite(PETSR2_LEVER_L, LOW);
  digitalWrite(PETSR2_LEVER_R, LOW);
}

void Controller::rightRotation() {
  // Serial.println("rightRotation");
  digitalWrite(PETSR2_POWER, HIGH);

  digitalWrite(PETSR2_LEVER_L, HIGH);
  digitalWrite(PETSR2_LEVER_R, LOW);
}

void Controller::leftRotation() {
  // Serial.println("leftRotation");  
  digitalWrite(PETSR2_POWER, HIGH);

  digitalWrite(PETSR2_LEVER_L, LOW);
  digitalWrite(PETSR2_LEVER_R, HIGH);
}
