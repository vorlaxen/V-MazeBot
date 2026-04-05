#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <Arduino.h>

enum class MoveAction {
    FORWARD,
    TURN_LEFT,
    TURN_RIGHT,
    U_TURN,
    STOP
};

enum class NavStrategy {
    RIGHT_HAND,
    LEFT_HAND
};

class Navigation {
public:
    virtual ~Navigation() {}
    virtual MoveAction decide(int left, int right, int fLeft, int fRight) = 0;
    void setThreshold(int threshold) { _threshold = threshold; }

protected:
    int _threshold = 600;
};

#endif