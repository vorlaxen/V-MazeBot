#ifndef RIGHT_HAND_RULE_H
#define RIGHT_HAND_RULE_H

#include "Navigation.h"

class RightHandRule : public Navigation {
public:
MoveAction decide(int left, int right, int fLeft, int fRight) override {
        // 1. Sağ taraf müsaitse hemen oraya yönel (Sağ El Kuralı Altın Kural)
        if (right < _threshold) {
            return MoveAction::TURN_RIGHT;
        }
    
        // 2. Sağ kapalı ama ön açık mı?
        // Not: fLeft ve fRight değerlerinden herhangi biri eşiği geçerse "ön kapalı" sayılmalı
        if (fLeft < _threshold && fRight < _threshold) {
            return MoveAction::FORWARD;
        }
    
        // 3. Sağ ve ön kapalı, bari sola dönelim
        if (left < _threshold) {
            return MoveAction::TURN_LEFT;
        }
    
        // 4. Çıkmaz sokak (Dead end)
        return MoveAction::U_TURN;
    }
};

#endif