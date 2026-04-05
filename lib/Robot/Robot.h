#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include "Sensors.h"
#include "MotorControl.h"
#include "PIDControl.h"
#include "Navigation.h"

class Robot
{
public:
    Robot();
    void begin();
    void update();

private:
    Sensors sensors;
    MotorControl motor;
    PIDControl wallPID;

    Navigation* currentRule;

    unsigned long lastTime;

    void executeTurn(MoveAction action);
};

#endif