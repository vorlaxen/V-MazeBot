#pragma once
#include <Arduino.h>

<<<<<<< HEAD
class PIDControl {
private:
    float Kp, Ki, Kd;

    float prevError = 0;
    float integral = 0;
    float prevDerivative = 0;

    float integralLimit = 500;

    float outputMin, outputMax;

public:

    PIDControl(float kp, float kd, float ki = 0,
               float outMin = -255, float outMax = 255)
        : Kp(kp), Ki(ki), Kd(kd),
          outputMin(outMin), outputMax(outMax) {}

    float compute(float currentValue, float targetValue, unsigned long dtMs)
    {
        if (dtMs < 1) return 0;

        float dt = dtMs * 0.001f;

        float error = targetValue - currentValue;

        // Deadband
        if (abs(error) < 5) error = 0;

        // Integral
        integral += error * dt;
        integral = constrain(integral, -integralLimit, integralLimit);

        // Derivative
        float derivative = (error - prevError) / dt;

        // Low pass filter
        derivative = 0.7f * derivative + 0.3f * prevDerivative;
        prevDerivative = derivative;

        // Adaptive PID
        float scale = 1.0f + abs(error) / 512.0f;

        float output =
            (Kp * scale) * error +
            Ki * integral +
            (Kd * scale) * derivative;

        output = constrain(output, outputMin, outputMax);

        prevError = error;

        return output;
    }

    void reset()
    {
        prevError = 0;
        integral = 0;
        prevDerivative = 0;
    }

    void setTunings(float kp, float kd, float ki = 0)
    {
        Kp = kp;
        Kd = kd;
        Ki = ki;
    }
};
=======
PIDControl::PIDControl(float kp, float kd, float ki, float outMin, float outMax)
    : Kp(kp), Ki(ki), Kd(kd), outputMin(outMin), outputMax(outMax) {
    prevError = 0;
    integral = 0;
    prevDerivative = 0;
    integralLimit = 500;
}

float PIDControl::compute(float currentValue, float targetValue, unsigned long dtMs) {
    if (dtMs < 1) return 0;

    float dt = dtMs * 0.001f; // ms -> s
    float error = targetValue - currentValue;

    // Deadband
    if (abs(error) < 5) error = 0;

    // Integral
    integral += error * dt;
    integral = constrain(integral, -integralLimit, integralLimit);

    // Derivative
    float derivative = (error - prevError) / dt;
    derivative = 0.7f * derivative + 0.3f * prevDerivative; // Low-pass filter
    prevDerivative = derivative;

    // Adaptive PID
    float scale = 1.0f + abs(error) / 512.0f;
    float output = (Kp * scale) * error + Ki * integral + (Kd * scale) * derivative;

    output = constrain(output, outputMin, outputMax);
    prevError = error;

    return output;
}

void PIDControl::reset() {
    prevError = 0;
    integral = 0;
    prevDerivative = 0;
}

void PIDControl::setTunings(float kp, float kd, float ki) {
    Kp = kp;
    Kd = kd;
    Ki = ki;
}
>>>>>>> 32222e0 (XXXX)
