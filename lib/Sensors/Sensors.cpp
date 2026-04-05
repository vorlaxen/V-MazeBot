#include "Sensors.h"

#define EMIT_FRONT_L A4
#define EMIT_FRONT_R A5
#define EMIT_SIDE 13

Sensors::Sensors(int fl, int fr, int l, int r)
{
    pinFL = fl;
    pinFR = fr;
    pinL = l;
    pinR = r;
}

void Sensors::begin()
{
    pinMode(pinFL, INPUT);
    pinMode(pinFR, INPUT);
    pinMode(pinL, INPUT);
    pinMode(pinR, INPUT);

    pinMode(EMIT_FRONT_L, OUTPUT);
    pinMode(EMIT_FRONT_R, OUTPUT);
    pinMode(EMIT_SIDE, OUTPUT);
}

void Sensors::calibrate(int samples)
{
    long sumFL = 0;
    long sumFR = 0;
    long sumL = 0;
    long sumR = 0;

    for(int i=0;i<samples;i++)
    {
        update();

        sumFL += valFL;
        sumFR += valFR;
        sumL += valL;
        sumR += valR;

        delay(2);
    }

    baseFL = sumFL / samples;
    baseFR = sumFR / samples;
    baseL  = sumL / samples;
    baseR  = sumR / samples;
}

void Sensors::update()
{
    digitalWrite(EMIT_FRONT_L, HIGH);
    digitalWrite(EMIT_FRONT_R, HIGH);

    delayMicroseconds(80);

    valFL = analogRead(pinFL);
    valFR = analogRead(pinFR);

    digitalWrite(EMIT_FRONT_L, LOW);
    digitalWrite(EMIT_FRONT_R, LOW);

    digitalWrite(EMIT_SIDE, HIGH);

    delayMicroseconds(80);

    valL = analogRead(pinL);
    valR = analogRead(pinR);

    digitalWrite(EMIT_SIDE, LOW);
}

int Sensors::frontLeft(){ return valFL; }
int Sensors::frontRight(){ return valFR; }
int Sensors::left(){ return valL; }
int Sensors::right(){ return valR; }

float Sensors::getCenterError()
{
    return (valL - valR) * 0.01;
}