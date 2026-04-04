#include "MotorControl.h"
#include "Constants.h"

void MotorControl::begin() {
<<<<<<< HEAD
    pinMode(L_PWM, OUTPUT);
    pinMode(L_IN1, OUTPUT);
    pinMode(L_IN2, OUTPUT);

    pinMode(R_PWM, OUTPUT);
    pinMode(R_IN1, OUTPUT);
    pinMode(R_IN2, OUTPUT);
=======
  pinMode(L_PWM, OUTPUT);
  pinMode(L_IN1, OUTPUT);
  pinMode(L_IN2, OUTPUT);

  pinMode(R_PWM, OUTPUT);
  pinMode(R_IN1, OUTPUT);
  pinMode(R_IN2, OUTPUT);
>>>>>>> 32222e0 (XXXX)
}

void MotorControl::rampMotor(int &currentSpeed, int targetSpeed, int step) {
  if (currentSpeed < targetSpeed) {
    currentSpeed += step;
    if (currentSpeed > targetSpeed)
      currentSpeed = targetSpeed;
  } else if (currentSpeed > targetSpeed) {
    currentSpeed -= step;
    if (currentSpeed < targetSpeed)
      currentSpeed = targetSpeed;
  }
  currentSpeed = constrain(currentSpeed, -255, 255);
}

void MotorControl::setMotorSpeed(int leftSpeed, int rightSpeed) {
<<<<<<< HEAD
    leftSpeed = constrain(leftSpeed, -255, 255);
    rightSpeed = constrain(rightSpeed, -255, 255);
    // LEFT MOTOR
    if(leftSpeed >= 0) {
        digitalWrite(L_IN1, HIGH);
        digitalWrite(L_IN2, LOW);
        analogWrite(L_PWM, leftSpeed);
    } else {
        digitalWrite(L_IN1, LOW);
        digitalWrite(L_IN2, HIGH);
        analogWrite(L_PWM, abs(leftSpeed));
    }

    // RIGHT MOTOR
    if(rightSpeed >= 0) {
        digitalWrite(R_IN1, HIGH);
        digitalWrite(R_IN2, LOW);
        analogWrite(R_PWM, rightSpeed);
    } else {
        digitalWrite(R_IN1, LOW);
        digitalWrite(R_IN2, HIGH);
        analogWrite(R_PWM, abs(rightSpeed));
    }
=======
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);
  // LEFT MOTOR
  if (leftSpeed >= 0) {
    digitalWrite(L_IN1, HIGH);
    digitalWrite(L_IN2, LOW);
    analogWrite(L_PWM, leftSpeed);
  } else {
    digitalWrite(L_IN1, LOW);
    digitalWrite(L_IN2, HIGH);
    analogWrite(L_PWM, abs(leftSpeed));
  }

  // RIGHT MOTOR
  if (rightSpeed >= 0) {
    digitalWrite(R_IN1, HIGH);
    digitalWrite(R_IN2, LOW);
    analogWrite(R_PWM, rightSpeed);
  } else {
    digitalWrite(R_IN1, LOW);
    digitalWrite(R_IN2, HIGH);
    analogWrite(R_PWM, abs(rightSpeed));
  }
>>>>>>> 32222e0 (XXXX)
}

void MotorControl::forward(int targetSpeed) {
  rampMotor(currentLeft, targetSpeed, 5);
  rampMotor(currentRight, targetSpeed, 5);
  setMotorSpeed(currentLeft, currentRight);
}

void MotorControl::backward(int targetSpeed) {
  rampMotor(currentLeft, -targetSpeed, 5);
  rampMotor(currentRight, -targetSpeed, 5);
  setMotorSpeed(currentLeft, currentRight);
}

void MotorControl::pivotLeft(int targetSpeed) {
<<<<<<< HEAD
    rampMotor(currentRight, targetSpeed, 10);
    rampMotor(currentLeft, 0, 10);
    setMotorSpeed(currentLeft, currentRight);
}

void MotorControl::pivotRight(int targetSpeed) {
    rampMotor(currentLeft, targetSpeed, 10);
    rampMotor(currentRight, 0, 10);
    setMotorSpeed(currentLeft, currentRight);
=======
  rampMotor(currentRight, targetSpeed, 10);
  rampMotor(currentLeft, 0, 10);
  setMotorSpeed(currentLeft, currentRight);
}

void MotorControl::pivotRight(int targetSpeed) {
  rampMotor(currentLeft, targetSpeed, 10);
  rampMotor(currentRight, 0, 10);
  setMotorSpeed(currentLeft, currentRight);
>>>>>>> 32222e0 (XXXX)
}

void MotorControl::tankTurnLeft(int targetSpeed) {
  rampMotor(currentLeft, -targetSpeed, 10);
  rampMotor(currentRight, targetSpeed, 10);
  setMotorSpeed(currentLeft, currentRight);
}

void MotorControl::tankTurnRight(int targetSpeed) {
  rampMotor(currentLeft, targetSpeed, 10);
  rampMotor(currentRight, -targetSpeed, 10);
  setMotorSpeed(currentLeft, currentRight);
}