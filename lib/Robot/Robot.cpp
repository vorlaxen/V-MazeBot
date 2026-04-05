#include "Robot.h"
#include "Pins.h"
#include "./rules/RightHandRule.h"

#define BASE_SPEED 120

Robot::Robot() :
    sensors(Pins::REC_FL, Pins::REC_FR, Pins::REC_L, Pins::REC_R),
    wallPID(1.5, 0.5, 0.01, -100, 100)
{
    lastTime = 0;
}

void Robot::begin()
{
    Serial.begin(115200);

    motor.begin();
    sensors.begin();
    sensors.calibrate(1000);

    currentRule = new RightHandRule();
    currentRule->setThreshold(700);

    state = RobotState::DRIVE;
}

void Robot::update()
{
    unsigned long now = millis();
    unsigned long dt = now - lastTime;

    if (dt < 20) return;
    lastTime = now;

    sensors.update();

    if(state == RobotState::TURNING)
    {
        handleTurn(now);
        return;
    }

    MoveAction nextAction = currentRule->decide(
        sensors.left(),
        sensors.right(),
        sensors.frontLeft(),
        sensors.frontRight()
    );

    if(nextAction != MoveAction::FORWARD)
    {
        startTurn(nextAction);
        return;
    }

    float error = calculateWallError();

    if(error == 0)
    {
        motor.setMotorSpeed(BASE_SPEED, BASE_SPEED);
        wallPID.reset();
    }
    else
    {
        float correction = wallPID.compute(error, 0, dt);

        int left = constrain(BASE_SPEED - correction, -255, 255);
        int right = constrain(BASE_SPEED + correction, -255, 255);

        motor.setMotorSpeed(left, right);
    }
}

void Robot::startTurn(MoveAction action)
{
    state = RobotState::TURNING;
    turnAction = action;
    turnStart = millis();

    if(action == MoveAction::TURN_RIGHT)
    {
        motor.tankTurnRight(120);
        turnDuration = 380;
    }
    else if(action == MoveAction::TURN_LEFT)
    {
        motor.tankTurnLeft(120);
        turnDuration = 380;
    }
    else if(action == MoveAction::U_TURN)
    {
        motor.tankTurnRight(120);
        turnDuration = 750;
    }

    wallPID.reset();
}

void Robot::handleTurn(unsigned long now)
{
    if(now - turnStart >= turnDuration)
    {
        state = RobotState::DRIVE;
        motor.setMotorSpeed(BASE_SPEED, BASE_SPEED);
    }
}

float Robot::calculateWallError()
{
    int left = sensors.left();
    int right = sensors.right();
    int frontLeft = sensors.frontLeft();
    int frontRight = sensors.frontRight();

    wallMask = 0;

    const int TARGET = 300;
    const int onWallThreshold = 250;
    const int sideWallThreshold = 200;

    if(frontLeft > onWallThreshold || frontRight > onWallThreshold)
        wallMask |= 0b010;

    if(left > sideWallThreshold)
        wallMask |= 0b100;

    if(right > sideWallThreshold)
        wallMask |= 0b001;

    if(wallMask == 0b101)
        return (right - left) * 0.5;

    else if(wallMask & 0b001)
        return right - TARGET;

    else if(wallMask & 0b100)
        return TARGET - left;

    else
        return 0;
}