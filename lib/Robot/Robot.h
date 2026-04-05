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
    // Hardware modules
    Sensors sensors;
    MotorControl motor;
    PIDControl wallPID;

    // Navigation rule
    Navigation* currentRule;

    // Loop timing
    unsigned long lastTime;

    // Robot state machine
    enum class RobotState
    {
        DRIVE,
        TURNING
    };

    RobotState state;

    // Turn control
    MoveAction turnAction;
    unsigned long turnStart;
    unsigned long turnDuration;
    byte wallMask;

    // Turn functions
    void startTurn(MoveAction action);
    void handleTurn(unsigned long now);
    float calculateWallError();
};

#endif