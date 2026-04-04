#pragma once
#include <Arduino.h>

namespace Pins {
    // Motor Pins
    constexpr uint8_t PWM_L = 6;
    constexpr uint8_t IN2_L = 7;
    constexpr uint8_t IN1_L = 8;
    
    constexpr uint8_t IN1_R = 9;
    constexpr uint8_t IN2_R = 10;
    constexpr uint8_t PWM_R = 11;

    // Sensor Emitter Pins
    constexpr uint8_t EMIT_FL = A4;
    constexpr uint8_t EMIT_FR = A5;
    constexpr uint8_t EMIT_SIDES = 13;
    constexpr uint8_t QTR_ON = 12; // Emitter Power?

    // Sensor Receiver Pins
    constexpr uint8_t REC_FL = A0;
    constexpr uint8_t REC_L = A1;
    constexpr uint8_t REC_R = A2;
    constexpr uint8_t REC_FR = A3;
}

namespace Config {
    // Sensor Thresholds
    constexpr int THRESH_WALL_FRONT = 250;
    constexpr int THRESH_WALL_SIDE = 200;
    constexpr int TARGET_DIST_SIDE = 300;

    // Speeds
    constexpr int SPEED_BASE = 70;
    constexpr int SPEED_TURN = 80;

    // PID Tunings for Wall Follow
    constexpr float WALL_KP = 0.18f;
    constexpr float WALL_KI = 0.00005f;
    constexpr float WALL_KD = 0.28f;
    constexpr float WALL_PID_LIMIT = 40.0f;
    constexpr float WALL_I_LIMIT = 300.0f;
}