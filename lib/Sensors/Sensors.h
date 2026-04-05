#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

class Sensors
{
public:
    Sensors(int fl, int fr, int l, int r);

    void begin();
    void calibrate(int samples);
    void update();

    int frontLeft();
    int frontRight();
    int left();
    int right();

    float getCenterError();

private:
    int pinFL, pinFR, pinL, pinR;
    int valFL, valFR, valL, valR;
    int baseFL, baseFR, baseL, baseR;

    int readIR(int pin, int emitPin);
};

#endif