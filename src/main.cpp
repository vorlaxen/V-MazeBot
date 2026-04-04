#include <Arduino.h>
#include "MotorControl.h"
#include "PIDControl.h"

// Nesneler
MotorControl motor;
PIDControl wallPID(1.5, 0.5, 0.01, -100, 100); // Duvar ortalama PID'si

// Sensör Pinleri
const int L_IR = A0, R_IR = A1, FL_IR = A2, FR_IR = A3;

const int FRONT_CRITICAL = 800;  // Ön duvara çok yakın (Dönme zamanı)
const int BASE_SPEED = 150;      // Genel hız
const int TURN_SPEED = 120;      // Dönüş hızı

unsigned long lastTime = 0;

void setup() {
    Serial.begin(115200);
    motor.begin();
}

void loop() {
    unsigned long now = millis();
    unsigned long dt = now - lastTime;
    if (dt < 20) return; // 50Hz çalışma hızı
    lastTime = now;

    // 1. Sensörleri Oku
    int leftDist  = analogRead(Pins::L_IR);
    int rightDist = analogRead(Pins::R_IR);
    int fLeft     = analogRead(Pins::FL_IR);
    int fRight    = analogRead(Pins::FR_IR);
    int frontMax  = max(fLeft, fRight);

    // 2. Karar Verme (Logic)
    
    // DURUM A: ÖNÜM KAPALI (Dönmem lazım)
    if (frontMax > FRONT_CRITICAL) {
        motor.setMotorSpeed(0, 0); // Kısa bir an dur
        delay(100);

        // Hangi yöne döneceğine karar ver
        if (leftDist < rightDist) {
            // Sol daha boş, sola dön
            while(analogRead(FL_IR) > 400) { // Önü açılana kadar dön
                motor.tankTurnLeft(TURN_SPEED);
                delay(10);
            }
        } else {
            // Sağ daha boş, sağa dön
            while(analogRead(FR_IR) > 400) {
                motor.tankTurnRight(TURN_SPEED);
                delay(10);
            }
        }
        wallPID.reset(); // PID'yi sıfırla ki eski hatalar yeni yola karışmasın
    } 
    
    // DURUM B: YOL AÇIK (PID ile Duvar Takibi)
    else {
        float error = leftDist - rightDist;
        float correction = wallPID.compute(error, 0, dt);

        // Hedef hızları belirle
        int leftTarget = BASE_SPEED - correction;
        int rightTarget = BASE_SPEED + correction;

        // RampMotor kullanarak yumuşak sürüş yap
        motor.setMotorSpeed(leftTarget, rightTarget);
    }
}