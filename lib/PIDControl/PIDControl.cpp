#include "PIDControl.h"
#include <math.h>

PIDControl::PIDControl(float kp, float kd, float ki, float outMin, float outMax)
{
    Kp = kp; Kd = kd; Ki = ki;
    outputMin = outMin;
    outputMax = outMax;

    integralLimit = 400.0f;
    deadband = 0.1f;
    derivativeFilter = 0.6f;

    reset();
}

float PIDControl::compute(float currentValue, float targetValue, unsigned long dtMs)
{
    if (dtMs == 0) return 0;
    float dt = dtMs * 0.001f;

    float error = targetValue - currentValue;

    if (fabs(error) < deadband) error = 0;

    float pTerm = Kp * error;
    float rawDerivative = (currentValue - prevValue) / dt;
    float derivative = (derivativeFilter * prevDerivative) + ((1.0f - derivativeFilter) * rawDerivative);
    float dTerm = -Kd * derivative;

    float outputWithoutI = pTerm + dTerm;
    if (outputWithoutI < outputMax && outputWithoutI > outputMin)
    {
        integral += error * dt;
        integral = constrain(integral, -integralLimit, integralLimit);
    }
    float iTerm = Ki * integral;

    float output = pTerm + iTerm + dTerm;
    output = constrain(output, outputMin, outputMax);

    prevValue = currentValue; 
    prevDerivative = derivative;

    return output;
}

void PIDControl::reset()
{
    prevValue = 0;
    integral = 0;
    prevDerivative = 0;
}