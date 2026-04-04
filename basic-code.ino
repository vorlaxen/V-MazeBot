#include <Arduino.h>

// --- Pin Tanımlamaları ---
#define PWM_Sol 6
#define IN2_Sol 7
#define IN1_Sol 8
#define IN1_Sag 9
#define IN2_Sag 10
#define PWM_Sag 11

#define Sol_on_alici  A0
#define Sol_yan_alici A1
#define Sag_yan_alici A2
#define Sag_on_alici  A3
#define Sol_on_verici A4
#define Yan_vericiler 13
#define Sag_on_verici A5

#define Qtr_ON 12

// --- EŞİK DEĞERLER ---
const int onDuvar_esik = 250;
const int yanDuvar_esik = 200;
const int Hedef_Mesafe = 300;

// --- PID ---
float Kp = 0.18;
float Kd = 0.28;
float Ki = 0.00005;

int HIZ = 70;

float I_birikim = 0;
int sonhata = 0;

int SolOA, SagOA, SolYA, SagYA;
byte duvarlar = 0;

void setup() {

  pinMode(PWM_Sol, OUTPUT);
  pinMode(IN1_Sol, OUTPUT);
  pinMode(IN2_Sol, OUTPUT);

  pinMode(PWM_Sag, OUTPUT);
  pinMode(IN1_Sag, OUTPUT);
  pinMode(IN2_Sag, OUTPUT);

  pinMode(Sol_on_verici, OUTPUT);
  pinMode(Yan_vericiler, OUTPUT);
  pinMode(Sag_on_verici, OUTPUT);

  pinMode(Qtr_ON, OUTPUT);

  digitalWrite(Qtr_ON, HIGH);

  Serial.begin(115200);

  delay(2000);
}

void Sensor_oku() {

  digitalWrite(Sag_on_verici, HIGH);
  digitalWrite(Sol_on_verici, HIGH);
  delayMicroseconds(100);

  SagOA = analogRead(Sag_on_alici);
  SolOA = analogRead(Sol_on_alici);

  digitalWrite(Sag_on_verici, LOW);
  digitalWrite(Sol_on_verici, LOW);

  digitalWrite(Yan_vericiler, HIGH);
  delayMicroseconds(100);

  SolYA = analogRead(Sol_yan_alici);
  SagYA = analogRead(Sag_yan_alici);

  digitalWrite(Yan_vericiler, LOW);

  duvarlar = 0;

  if (SolOA > onDuvar_esik || SagOA > onDuvar_esik) duvarlar |= 0b010;
  if (SolYA > yanDuvar_esik) duvarlar |= 0b100;
  if (SagYA > yanDuvar_esik) duvarlar |= 0b001;
}

void Motor_Yaz(int sol, int sag) {

  sol = constrain(sol, -255, 255);
  sag = constrain(sag, -255, 255);

  if (sol >= 0) {
    digitalWrite(IN1_Sol, HIGH);
    digitalWrite(IN2_Sol, LOW);
    analogWrite(PWM_Sol, sol);
  }
  else {
    digitalWrite(IN1_Sol, LOW);
    digitalWrite(IN2_Sol, HIGH);
    analogWrite(PWM_Sol, -sol);
  }

  if (sag >= 0) {
    digitalWrite(IN1_Sag, HIGH);
    digitalWrite(IN2_Sag, LOW);
    analogWrite(PWM_Sag, sag);
  }
  else {
    digitalWrite(IN1_Sag, LOW);
    digitalWrite(IN2_Sag, HIGH);
    analogWrite(PWM_Sag, -sag);
  }
}

void PID_Surus() {

  int hata = 0;

  if ((duvarlar & 0b101) == 0b101) {
    hata = (SagYA - SolYA) / 2;
  }

  else if (duvarlar & 0b001) {
    hata = SagYA - Hedef_Mesafe;
  }

  else if (duvarlar & 0b100) {
    hata = Hedef_Mesafe - SolYA;
  }

  else {
    Motor_Yaz(HIZ, HIZ);
    return;
  }

  I_birikim += hata;
  I_birikim = constrain(I_birikim, -300, 300);

  int D = hata - sonhata;

  int duzeltme =
  (hata * Kp) +
  (I_birikim * Ki) +
  (D * Kd);

  duzeltme = constrain(duzeltme, -40, 40);

  sonhata = hata;

  int solMotor = HIZ + duzeltme;
  int sagMotor = HIZ - duzeltme;

  Motor_Yaz(solMotor, sagMotor);
}

void Sert_Donus(int yon) {

  unsigned long baslangic = millis();

  while (true) {

    if (yon == 1)
      Motor_Yaz(80, -80);

    else
      Motor_Yaz(-80, 80);

    delay(5);

    Sensor_oku();

    if (SolOA < (onDuvar_esik - 40) &&
        SagOA < (onDuvar_esik - 40)) {
      break;
    }

    if (millis() - baslangic > 1200)
      break;
  }

  Motor_Yaz(HIZ, HIZ);
  delay(120);

  I_birikim = 0;
  sonhata = 0;
}

void loop() {

  Sensor_oku();

  if (duvarlar & 0b010) {

    Motor_Yaz(0, 0);
    delay(80);

    if (SolYA < SagYA)
      Sert_Donus(-1);

    else
      Sert_Donus(1);
  }

  else {

    PID_Surus();
  }
}