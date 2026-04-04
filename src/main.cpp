#include <Arduino.h>
#include "../lib/MotorControl/MotorControl.h"
#include "../lib/PIDControl/PIDControl.h"

MotorControl motor;
PIDControl wallPID(1.2, 0.5, 0.0, -100, 100);   // Wall-follow PID
PIDControl speedPID_L(2.0, 0.3, 0.0, -255, 255); // Sol motor hız PID
PIDControl speedPID_R(2.0, 0.3, 0.0, -255, 255); // Sağ motor hız PID

unsigned long lastTime = 0;

void setup() {
  Serial.begin(115200);
  motor.begin();
}

void loop() {
  unsigned long now = millis();
  unsigned long dt = now - lastTime;
  if (dt < 10) return; // Min 10 ms

  lastTime = now;

  int leftSensor  = analogRead(A0);
  int rightSensor = analogRead(A1);

  float wallError = leftSensor - rightSensor;
  float wallCorrection = wallPID.compute(wallError, 0, dt); // hedef fark = 0

  int baseSpeed = 150;
  int targetLeft  = baseSpeed - wallCorrection;
  int targetRight = baseSpeed + wallCorrection;

  int currentLeft  = motor.getCurrentLeft();
  int currentRight = motor.getCurrentRight();
  int pwmLeft  = speedPID_L.compute(currentLeft, targetLeft, dt);
  int pwmRight = speedPID_R.compute(currentRight, targetRight, dt);

  motor.setMotorSpeed(pwmLeft, pwmRight);
}