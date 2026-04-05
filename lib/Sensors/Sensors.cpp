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

    for(int i = 0; i < samples; i++)
    {
        update();

        sumFL += valFL;
        sumFR += valFR;
        sumL  += valL;
        sumR  += valR;

        delay(2);
    }

    baseFL = sumFL / samples;
    baseFR = sumFR / samples;
    baseL  = sumL  / samples;
    baseR  = sumR  / samples;
}

int Sensors::readIR(int pin, int emitPin)
{
    digitalWrite(emitPin, LOW);
    delayMicroseconds(50);
    int ambient = analogRead(pin);

    digitalWrite(emitPin, HIGH);
    delayMicroseconds(80);
    int reflect = analogRead(pin);

    digitalWrite(emitPin, LOW);

    int value = reflect - ambient;

    if(value < 0) value = 0;

    return value;
}

void Sensors::update()
{
    valFL = readIR(pinFL, EMIT_FRONT_L) - baseFL;
    valFR = readIR(pinFR, EMIT_FRONT_R) - baseFR;

    valL = readIR(pinL, EMIT_SIDE) - baseL;
    valR = readIR(pinR, EMIT_SIDE) - baseR;

    if(valFL < 0) valFL = 0;
    if(valFR < 0) valFR = 0;
    if(valL < 0) valL = 0;
    if(valR < 0) valR = 0;
}

int Sensors::frontLeft(){ return valFL; }
int Sensors::frontRight(){ return valFR; }
int Sensors::left(){ return valL; }
int Sensors::right(){ return valR; }

float Sensors::getCenterError()
{
    float sum = valL + valR + 1;
    return (float)(valL - valR) / sum;
}