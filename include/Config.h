#pragma once
#include <Arduino.h>

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