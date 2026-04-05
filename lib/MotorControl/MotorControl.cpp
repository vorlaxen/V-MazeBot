#include "MotorControl.h"
#include "Pins.h"
#include "Config.h"

void MotorControl::begin() {
    pinMode(Pins::PWM_L, OUTPUT);
    pinMode(Pins::IN1_L, OUTPUT);
    pinMode(Pins::IN2_L, OUTPUT);

    pinMode(Pins::PWM_R, OUTPUT);
    pinMode(Pins::IN1_R, OUTPUT);
    pinMode(Pins::IN2_R, OUTPUT);
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
    leftSpeed = constrain(leftSpeed, -255, 255);
    rightSpeed = constrain(rightSpeed, -255, 255);
    
    // Sol Motor Kontrolü
    if(leftSpeed >= 0) {
        digitalWrite(Pins::IN1_L, HIGH);
        digitalWrite(Pins::IN2_L, LOW);
        analogWrite(Pins::PWM_L, leftSpeed);
    } else {
        digitalWrite(Pins::IN1_L, LOW);
        digitalWrite(Pins::IN2_L, HIGH);
        analogWrite(Pins::PWM_L, abs(leftSpeed));
    }

    // Sağ Motor Kontrolü
    if(rightSpeed >= 0) {
        digitalWrite(Pins::IN1_R, HIGH);
        digitalWrite(Pins::IN2_R, LOW);
        analogWrite(Pins::PWM_R, rightSpeed);
    } else {
        digitalWrite(Pins::IN1_R, LOW);
        digitalWrite(Pins::IN2_R, HIGH);
        analogWrite(Pins::PWM_R, abs(rightSpeed));
    }
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
    rampMotor(currentRight, targetSpeed, 10);
    rampMotor(currentLeft, 0, 10);
    setMotorSpeed(currentLeft, currentRight);
}

void MotorControl::pivotRight(int targetSpeed) {
    rampMotor(currentLeft, targetSpeed, 10);
    rampMotor(currentRight, 0, 10);
    setMotorSpeed(currentLeft, currentRight);
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