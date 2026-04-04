#pragma once
#include <Arduino.h>

class MotorControl {
public:
    void begin();
    
    // Sets speeds for left and right motors: bounded [-255, 255]
    void setSpeeds(int leftSpeed, int rightSpeed);

    // Stops both motors immediately
    void stop();
};