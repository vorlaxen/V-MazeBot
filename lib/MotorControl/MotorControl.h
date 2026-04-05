#pragma once
#include <Arduino.h>

class MotorControl {
public:
    void begin();
    
    // Temel hız ayarları (Main.cpp ve iç fonksiyonlar kullanır)
    void setSpeeds(int leftSpeed, int rightSpeed);
    void setMotorSpeed(int leftSpeed, int rightSpeed); 

    // Hareket Fonksiyonları
    void forward(int targetSpeed);
    void backward(int targetSpeed);
    void pivotLeft(int targetSpeed);
    void pivotRight(int targetSpeed);
    void tankTurnLeft(int targetSpeed);
    void tankTurnRight(int targetSpeed);
    
    void stop();

private:
void rampMotor(int &currentSpeed, int targetSpeed, int step);

    int currentLeft = 0;
    int currentRight = 0;
};