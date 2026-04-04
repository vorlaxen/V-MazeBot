#pragma once
#include <Arduino.h>

// Motor pins
constexpr int L_PWM = 6;
constexpr int L_IN1 = 8;
constexpr int L_IN2 = 7;

constexpr int R_PWM = 11;
constexpr int R_IN1 = 9;
constexpr int R_IN2 = 10;

// Sensor pins
constexpr int S_FRONT_L = A0;
constexpr int S_LEFT    = A1;
constexpr int S_RIGHT   = A2;
constexpr int S_FRONT_R = A3;

// IR emitters
constexpr int IR_FRONT_L = A4;
constexpr int IR_FRONT_R = A5;
constexpr int IR_SIDE    = 13;

// Robot speed
constexpr int BASE_SPEED = 60;

// PID
constexpr float KP = 0.3f;
constexpr float KD = 0.5f;
constexpr float KI = 0.0001f;

// Sensor thresholds
constexpr int FRONT_WALL = 250;
constexpr int SIDE_WALL  = 200;
constexpr int TARGET_DIST = 300;