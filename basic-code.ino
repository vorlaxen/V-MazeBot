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

// --- YÖN TANIMLAMALARI ---
#define KUZEY 0
#define DOGU  1
#define GUNEY 2
#define BATI  3

// --- HÜCRE DUVARLARI BİT MASKELERİ (Mutlak Yönler) ---
#define DUVAR_KUZEY 0b0001
#define DUVAR_DOGU  0b0010
#define DUVAR_GUNEY 0b0100
#define DUVAR_BATI  0b1000

// --- HARİTALAMA DEĞİŞKENLERİ ---
byte labirent[16][16];
byte flood_mesafe[16][16];
int robotX = 0;
int robotY = 0;
byte robotYon = KUZEY;

// --- AŞAMA TAKİBİ ---
bool kesifBitti = false;
bool yolHesaplandi = false;

// --- KOMUT DİZİSİ (HIZLI TUR İÇİN) ---
#define MAX_KOMUT 256
char komutlar[MAX_KOMUT];
int komutSayisi = 0;
int aktifKomut = 0;

// --- YENİ PID DEĞERLERİ (HIZLI TUR) ---
float Kp_hizli = 0.30;
float Kd_hizli = 0.50;
float Ki_hizli = 0.0001;
int HIZ_hizli = 150;

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

  // Labirent duvarlarını sıfırla ve flood mesafelerini başlangıçta 255 yap
  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 16; y++) {
      labirent[x][y] = 0; // Henüz hiçbir duvar bilinmiyor
      flood_mesafe[x][y] = 255;
    }
  }

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

void Saga_Don_90() {
  // TODO: Gelecekte MPU6050 veya Enkoder ile iyileştirilecek.
  // Şu an zaman bazlı 90 derece dönüş (80 birim hızla yaklaşık X ms, mekaniğe göre kalibre edilmeli)
  Motor_Yaz(80, -80);
  delay(400); // 90 derece dönüş süresi (örnektir, kalibre edilmelidir)
  Motor_Yaz(0, 0);
  delay(100);

  robotYon = (robotYon + 1) % 4; // Yönü güncelle: KUZEY -> DOGU -> GUNEY -> BATI

  I_birikim = 0;
  sonhata = 0;
}

void Sola_Don_90() {
  // TODO: Gelecekte MPU6050 veya Enkoder ile iyileştirilecek.
  Motor_Yaz(-80, 80);
  delay(400); // 90 derece dönüş süresi (örnektir, kalibre edilmelidir)
  Motor_Yaz(0, 0);
  delay(100);

  robotYon = (robotYon + 3) % 4; // Yönü güncelle: KUZEY -> BATI -> GUNEY -> DOGU

  I_birikim = 0;
  sonhata = 0;
}

void Donus_180() {
  Saga_Don_90();
  Saga_Don_90();
}

void Duvarlari_Haritaya_Isle() {
  byte mutlakDuvarlar = 0;

  // 0b010 -> Ön, 0b100 -> Sol, 0b001 -> Sağ
  bool onDuvarVar = (duvarlar & 0b010);
  bool solDuvarVar = (duvarlar & 0b100);
  bool sagDuvarVar = (duvarlar & 0b001);

  if (robotYon == KUZEY) {
    if (onDuvarVar) mutlakDuvarlar |= DUVAR_KUZEY;
    if (sagDuvarVar) mutlakDuvarlar |= DUVAR_DOGU;
    if (solDuvarVar) mutlakDuvarlar |= DUVAR_BATI;
  } else if (robotYon == DOGU) {
    if (onDuvarVar) mutlakDuvarlar |= DUVAR_DOGU;
    if (sagDuvarVar) mutlakDuvarlar |= DUVAR_GUNEY;
    if (solDuvarVar) mutlakDuvarlar |= DUVAR_KUZEY;
  } else if (robotYon == GUNEY) {
    if (onDuvarVar) mutlakDuvarlar |= DUVAR_GUNEY;
    if (sagDuvarVar) mutlakDuvarlar |= DUVAR_BATI;
    if (solDuvarVar) mutlakDuvarlar |= DUVAR_DOGU;
  } else if (robotYon == BATI) {
    if (onDuvarVar) mutlakDuvarlar |= DUVAR_BATI;
    if (sagDuvarVar) mutlakDuvarlar |= DUVAR_KUZEY;
    if (solDuvarVar) mutlakDuvarlar |= DUVAR_GUNEY;
  }

  labirent[robotX][robotY] |= mutlakDuvarlar;

  // Komşu hücrelerin duvarlarını da güncelle (çift yönlü duvar mantığı)
  // Eger benim kuzeyim kapaliysa, ustumdeki hucrenin guneyi kapalidir.
  if ((mutlakDuvarlar & DUVAR_KUZEY) && robotY < 15) labirent[robotX][robotY+1] |= DUVAR_GUNEY;
  if ((mutlakDuvarlar & DUVAR_DOGU) && robotX < 15) labirent[robotX+1][robotY] |= DUVAR_BATI;
  if ((mutlakDuvarlar & DUVAR_GUNEY) && robotY > 0) labirent[robotX][robotY-1] |= DUVAR_KUZEY;
  if ((mutlakDuvarlar & DUVAR_BATI) && robotX > 0) labirent[robotX-1][robotY] |= DUVAR_DOGU;
}

struct Koordinat {
  byte x;
  byte y;
};

void Flood_Fill() {
  // Bütün mesafeleri 255 (erişilemez) yap
  for(int i=0; i<16; i++){
    for(int j=0; j<16; j++){
      flood_mesafe[i][j] = 255;
    }
  }

  Koordinat kuyruk[256];
  int bas = 0;
  int son = 0;

  // Hedef hücreleri (Merkez) belirle ve kuyruğa ekle.
  // Çoğu 16x16 labirentte merkez 4 hücredir: (7,7), (7,8), (8,7), (8,8)
  // Bu hucrelerin mesafesini 0 atayip algoritmayi buradan dalga dalga disariya baslatiyoruz
  flood_mesafe[7][7] = 0; kuyruk[son++] = {7,7};
  flood_mesafe[7][8] = 0; kuyruk[son++] = {7,8};
  flood_mesafe[8][7] = 0; kuyruk[son++] = {8,7};
  flood_mesafe[8][8] = 0; kuyruk[son++] = {8,8};

  while(bas < son) {
    Koordinat suanki = kuyruk[bas++];
    int x = suanki.x;
    int y = suanki.y;
    byte mesafe = flood_mesafe[x][y];

    // Komşulara bak ve eğer duvar yoksa ve daha kısa bir yol bulunduysa güncelle
    byte hucrunun_duvarlari = labirent[x][y];

    // Kuzey komşusu
    if (!(hucrunun_duvarlari & DUVAR_KUZEY) && y < 15) {
      if (flood_mesafe[x][y+1] == 255) {
        flood_mesafe[x][y+1] = mesafe + 1;
        kuyruk[son++] = {x, y+1};
      }
    }
    // Doğu komşusu
    if (!(hucrunun_duvarlari & DUVAR_DOGU) && x < 15) {
      if (flood_mesafe[x+1][y] == 255) {
        flood_mesafe[x+1][y] = mesafe + 1;
        kuyruk[son++] = {x+1, y};
      }
    }
    // Güney komşusu
    if (!(hucrunun_duvarlari & DUVAR_GUNEY) && y > 0) {
      if (flood_mesafe[x][y-1] == 255) {
        flood_mesafe[x][y-1] = mesafe + 1;
        kuyruk[son++] = {x, y-1};
      }
    }
    // Batı komşusu
    if (!(hucrunun_duvarlari & DUVAR_BATI) && x > 0) {
      if (flood_mesafe[x-1][y] == 255) {
        flood_mesafe[x-1][y] = mesafe + 1;
        kuyruk[son++] = {x-1, y};
      }
    }
  }
}

int Bir_Sonraki_Gidilecek_Yon() {
  int x = robotX;
  int y = robotY;
  byte min_mesafe = 255;
  int gidilecek_yon = robotYon; // Varsayılan olarak şu anki yön

  byte hucrunun_duvarlari = labirent[x][y];

  // Hedef hücre etrafındaki komşuların mesafesine bakıp en küçük mesafeli olana yönlenmek
  // Kuzey
  if (!(hucrunun_duvarlari & DUVAR_KUZEY) && y < 15) {
    if (flood_mesafe[x][y+1] < min_mesafe) {
      min_mesafe = flood_mesafe[x][y+1];
      gidilecek_yon = KUZEY;
    }
  }
  // Doğu
  if (!(hucrunun_duvarlari & DUVAR_DOGU) && x < 15) {
    if (flood_mesafe[x+1][y] < min_mesafe) {
      min_mesafe = flood_mesafe[x+1][y];
      gidilecek_yon = DOGU;
    }
  }
  // Güney
  if (!(hucrunun_duvarlari & DUVAR_GUNEY) && y > 0) {
    if (flood_mesafe[x][y-1] < min_mesafe) {
      min_mesafe = flood_mesafe[x][y-1];
      gidilecek_yon = GUNEY;
    }
  }
  // Batı
  if (!(hucrunun_duvarlari & DUVAR_BATI) && x > 0) {
    if (flood_mesafe[x-1][y] < min_mesafe) {
      min_mesafe = flood_mesafe[x-1][y];
      gidilecek_yon = BATI;
    }
  }

  // Eğer eşit mesafede yönler varsa, robotun şu anki baktığı yöne (veya en az dönüş yapacağı yöne) öncelik verilebilir.
  // Bu basit implementasyonda ilk bulunan min_mesafe yönü alınır.
  return gidilecek_yon;
}

void Bir_Hucre_Ileri() {
  // 1 hücre boyutu kadar ileri gitme
  // Zaman ve hız veya PID kontrollü süreli gidiş
  unsigned long baslangic = millis();
  unsigned long hucreGecisSuresi = 600; // 1 hücre geçişi için gereken süre (örnek: 600ms)

  while(millis() - baslangic < hucreGecisSuresi) {
    Sensor_oku();
    PID_Surus();
  }

  Motor_Yaz(0, 0);
  delay(50);

  // Konumu güncelle
  if (robotYon == KUZEY) robotY++;
  else if (robotYon == DOGU) robotX++;
  else if (robotYon == GUNEY) robotY--;
  else if (robotYon == BATI) robotX--;
}

void Sert_Donus(int yon) {
  if (yon == 1) {
    Saga_Don_90();
  } else {
    Sola_Don_90();
  }
}

void Yolu_Hesapla() {
  int x = 0;
  int y = 0;
  int anlikYon = KUZEY; // Başlangıç yönü

  komutSayisi = 0;

  // Başlangıçtan (0,0) merkeze kadar sel dolgusunu tersten takip et
  while (!((x == 7 || x == 8) && (y == 7 || y == 8))) {
    byte min_mesafe = 255;
    int gidilecek_yon = anlikYon;

    byte hucrunun_duvarlari = labirent[x][y];

    if (!(hucrunun_duvarlari & DUVAR_KUZEY) && y < 15 && flood_mesafe[x][y+1] < min_mesafe) {
      min_mesafe = flood_mesafe[x][y+1];
      gidilecek_yon = KUZEY;
    }
    if (!(hucrunun_duvarlari & DUVAR_DOGU) && x < 15 && flood_mesafe[x+1][y] < min_mesafe) {
      min_mesafe = flood_mesafe[x+1][y];
      gidilecek_yon = DOGU;
    }
    if (!(hucrunun_duvarlari & DUVAR_GUNEY) && y > 0 && flood_mesafe[x][y-1] < min_mesafe) {
      min_mesafe = flood_mesafe[x][y-1];
      gidilecek_yon = GUNEY;
    }
    if (!(hucrunun_duvarlari & DUVAR_BATI) && x > 0 && flood_mesafe[x-1][y] < min_mesafe) {
      min_mesafe = flood_mesafe[x-1][y];
      gidilecek_yon = BATI;
    }

    // Yön değişikliği komutlarını oluştur
    if (gidilecek_yon != anlikYon) {
      if ((anlikYon + 1) % 4 == gidilecek_yon) {
        komutlar[komutSayisi++] = 'R'; // Sağ
      } else if ((anlikYon + 3) % 4 == gidilecek_yon) {
        komutlar[komutSayisi++] = 'L'; // Sol
      } else {
        komutlar[komutSayisi++] = 'B'; // Geri dön (veya iki kere sağ)
      }
    }

    komutlar[komutSayisi++] = 'F'; // İleri

    // Pozisyonu ve yönü güncelle
    anlikYon = gidilecek_yon;
    if (anlikYon == KUZEY) y++;
    else if (anlikYon == DOGU) x++;
    else if (anlikYon == GUNEY) y--;
    else if (anlikYon == BATI) x--;
  }

  // Hareketleri Birleştirme (Optimizasyon)
  // Pes pese gelen 'F' (Ileri) komutlari taranir ve tek bir sayi altinda birlestirilir (Orn: 5 kere F => '5')
  int optimizeSayisi = 0;
  char optimizeKomutlar[MAX_KOMUT];

  for(int i = 0; i < komutSayisi; i++) {
    if (komutlar[i] == 'F') {
      int sayac = 1;
      while(i + 1 < komutSayisi && komutlar[i+1] == 'F' && sayac < 9) {
        sayac++;
        i++;
      }
      optimizeKomutlar[optimizeSayisi++] = '0' + sayac; // '1', '2', '3' vs. hücre sayısı
    } else {
      optimizeKomutlar[optimizeSayisi++] = komutlar[i];
    }
  }

  komutSayisi = optimizeSayisi;
  for(int i = 0; i < komutSayisi; i++) {
    komutlar[i] = optimizeKomutlar[i];
  }
}

void loop() {

  if (!kesifBitti) {
    Sensor_oku();
    Motor_Yaz(0, 0); // Karar aşamasında dur
    delay(50);

    Duvarlari_Haritaya_Isle();
    Flood_Fill();

    // Merkezde miyiz?
    if ((robotX == 7 || robotX == 8) && (robotY == 7 || robotY == 8)) {
      kesifBitti = true;
      Motor_Yaz(0, 0);

      // Yol optimizasyonunu hesapla
      Yolu_Hesapla();
      yolHesaplandi = true;

      // Robotu fiziksel olarak başlangıca geri götürme mantığı burada çağrılabilir.
      // Şimdilik test amaçlı delay koyup, sanki başlangıca konmuş gibi farz edebiliriz.
      delay(5000);
      robotX = 0;
      robotY = 0;
      robotYon = KUZEY;

      // PID değerlerini hızlı sürüşe ayarla
      Kp = Kp_hizli;
      Kd = Kd_hizli;
      Ki = Ki_hizli;
      HIZ = HIZ_hizli;

      return; // Keşif bitti, diğer aşamaya geçilecek
    }

    int hedefYon = Bir_Sonraki_Gidilecek_Yon();

    // Yönelim ayarlama
    if (hedefYon != robotYon) {
      if ((robotYon + 1) % 4 == hedefYon) {
        Saga_Don_90();
      } else if ((robotYon + 3) % 4 == hedefYon) {
        Sola_Don_90();
      } else {
        Donus_180();
      }
    }

    Bir_Hucre_Ileri();
  } else if (yolHesaplandi) {
    // FAZ 4: Hızlı Tur (Fast Run)
    if (aktifKomut < komutSayisi) {
      char komut = komutlar[aktifKomut];

      if (komut == 'R') {
        Saga_Don_90();
        aktifKomut++;
      } else if (komut == 'L') {
        Sola_Don_90();
        aktifKomut++;
      } else if (komut == 'B') {
        Donus_180();
        aktifKomut++;
      } else if (komut >= '1' && komut <= '9') {
        int hucreSayisi = komut - '0';

        // Çoklu hücre ilerlemesi
        unsigned long baslangic = millis();
        unsigned long toplamSure = hucreSayisi * 600; // Örnek süre. Hız arttığı için ayarlanmalı.
        // Hızlı sürüşte süre daha kısa olacağı için bir çarpan eklenebilir. Örn: (hucreSayisi * 600 * 70) / 150

        while(millis() - baslangic < (toplamSure * 70) / HIZ_hizli) {
          Sensor_oku();
          PID_Surus(); // Yeni hızlı PID devrede
        }

        Motor_Yaz(0, 0);
        delay(50);

        // Konum güncellemesi hızlı modda da yapılabilir
        if (robotYon == KUZEY) robotY += hucreSayisi;
        else if (robotYon == DOGU) robotX += hucreSayisi;
        else if (robotYon == GUNEY) robotY -= hucreSayisi;
        else if (robotYon == BATI) robotX -= hucreSayisi;

        aktifKomut++;
      } else {
        aktifKomut++;
      }
    } else {
      // Hedefe ulaşıldı
      Motor_Yaz(0, 0);
      while(1); // Sonsuz duruş
    }
  }
}