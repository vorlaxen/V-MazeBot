#pragma once
#include <Arduino.h>

class PIDControl {
private:
    float Kp;
    float Ki;
    float Kd;

    float prevError;
    float integral;
    float prevDerivative;
    float integralLimit;
    float outputMin;
    float outputMax;

public:
    // Constructor
    PIDControl(float kp, float kd, float ki = 0, float outMin = -255, float outMax = 255);

    // PID hesaplama
    float compute(float currentValue, float targetValue, unsigned long dtMs);

    // PID reset
    void reset();

    // PID ayarları değiştirme
    void setTunings(float kp, float kd, float ki = 0);
};