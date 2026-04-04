#pragma once
#include <Arduino.h>

class PIDControl {
private:
    float Kp, Ki, Kd;

    float prevValue;
    float integral;
    float prevDerivative;

    float integralLimit;
    float outputMin, outputMax;
    float deadband;
    float derivativeFilter;

public:
    // Constructor: Varsayılan olarak Ki=0 ve PWM limitleri (-255, 255)
    PIDControl(float kp, float kd, float ki = 0, float outMin = -255, float outMax = 255);

    float compute(float currentValue, float targetValue, unsigned long dtMs);

    void reset();
    void setTunings(float kp, float kd, float ki);

    void setDeadband(float value) { deadband = value; }
    void setIntegralLimit(float limit) { integralLimit = limit; }
    void setDerivativeFilter(float filter) { derivativeFilter = constrain(filter, 0.0f, 1.0f); }
};