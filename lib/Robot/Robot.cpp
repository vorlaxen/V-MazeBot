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
}

void Robot::update()
{
    unsigned long now = millis();
    unsigned long dt = now - lastTime;

    if (dt < 20) return;
    lastTime = now;

    sensors.update();

    MoveAction nextAction = currentRule->decide(
        sensors.left(),
        sensors.right(),
        sensors.frontLeft(),
        sensors.frontRight()
    );

    switch (nextAction)
    {
        case MoveAction::FORWARD:
        {
            float error = sensors.getCenterError();
            float correction = wallPID.compute(error, 0, dt);

            motor.setMotorSpeed(
                BASE_SPEED + correction,
                BASE_SPEED - correction
            );
            break;
        }

        case MoveAction::TURN_RIGHT:
        case MoveAction::TURN_LEFT:
        case MoveAction::U_TURN:
            executeTurn(nextAction);
            break;

        case MoveAction::STOP:
            motor.setMotorSpeed(0,0);
            break;
    }
}

void Robot::executeTurn(MoveAction action)
{
    motor.setMotorSpeed(0,0);
    delay(50);

    if (action == MoveAction::TURN_RIGHT)
    {
        motor.tankTurnRight(120);
        delay(380);
    }
    else if (action == MoveAction::TURN_LEFT)
    {
        motor.tankTurnLeft(120);
        delay(380);
    }
    else if (action == MoveAction::U_TURN)
    {
        motor.tankTurnRight(120);
        delay(750);
    }

    motor.setMotorSpeed(0,0);
    wallPID.reset();
    delay(50);
}