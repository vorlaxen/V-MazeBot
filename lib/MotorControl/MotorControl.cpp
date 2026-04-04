#include "MotorControl.h"
#include "Constants.h"

void MotorControl::begin() {
    pinMode(Pins::PWM_L, OUTPUT);
    pinMode(Pins::IN1_L, OUTPUT);
    pinMode(Pins::IN2_L, OUTPUT);

    pinMode(Pins::PWM_R, OUTPUT);
    pinMode(Pins::IN1_R, OUTPUT);
    pinMode(Pins::IN2_R, OUTPUT);
}

void MotorControl::setSpeeds(int leftSpeed, int rightSpeed) {
    leftSpeed = constrain(leftSpeed, -255, 255);
    rightSpeed = constrain(rightSpeed, -255, 255);

    // Left Motor
    if (leftSpeed >= 0) {
        digitalWrite(Pins::IN1_L, HIGH);
        digitalWrite(Pins::IN2_L, LOW);
        analogWrite(Pins::PWM_L, leftSpeed);
    } else {
        digitalWrite(Pins::IN1_L, LOW);
        digitalWrite(Pins::IN2_L, HIGH);
        analogWrite(Pins::PWM_L, -leftSpeed);
    }

    // Right Motor
    if (rightSpeed >= 0) {
        digitalWrite(Pins::IN1_R, HIGH);
        digitalWrite(Pins::IN2_R, LOW);
        analogWrite(Pins::PWM_R, rightSpeed);
    } else {
        digitalWrite(Pins::IN1_R, LOW);
        digitalWrite(Pins::IN2_R, HIGH);
        analogWrite(Pins::PWM_R, -rightSpeed);
    }
}

void MotorControl::stop() {
    setSpeeds(0, 0);
}