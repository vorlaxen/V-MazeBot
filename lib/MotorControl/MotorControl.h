#pragma once
#include <Arduino.h>

class MotorControl {
public:
    void begin();

    void setMotorSpeed(int leftSpeed, int rightSpeed);

    void forward(int targetSpeed);
    void backward(int targetSpeed);

    void pivotLeft(int targetSpeed);
    void pivotRight(int targetSpeed);

    void tankTurnLeft(int targetSpeed);
    void tankTurnRight(int targetSpeed);

    int getCurrentLeft() { return currentLeft; }
    int getCurrentRight() { return currentRight; }

private:
    int currentLeft = 0;
    int currentRight = 0;

    void rampMotor(int &currentSpeed, int targetSpeed, int step);
};