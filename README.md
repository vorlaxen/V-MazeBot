# Micromouse: Flood Fill & PID Optimize Labirent Robotu

Bu proje, 16x16 bir micromouse labirentinde **en kısa yolu otonom olarak bulup maksimum hızla tamamlayan** Arduino Nano tabanlı bir robotik sistemdir.

Amaç: **Minimum kaynak kullanımı + maksimum karar hızı + stabil hareket**.

---

# Sistem Genel Bakış

* **Platform:** Arduino Nano (ATmega328P @ 16MHz)
* **Algoritma:** Dinamik Flood Fill
* **Kontrol:** PD (Proportional-Derivative)
* **Sürüş:** Diferansiyel (2 DC Motor)
* **Bellek Optimizasyonu:** Bitmask tabanlı harita
* **Labirent:** 16x16 hücre

---

# Sistem Mimarisi

Sistem 3 ana katmandan oluşur:

### 1. Algoritma Katmanı

* Flood Fill (mesafe hesaplama)
* Path decision logic

### 2. Kontrol Katmanı

* PD controller
* Motor hız düzeltmeleri

### 3. Donanım Katmanı

* IR sensörler
* Motor sürücüler
* Encoder (opsiyonel ama önerilir)

---

# Donanım Gereksinimleri

## Minimum

* Arduino Nano
* 2x DC motor + dişli kutusu
* Motor driver (TB6612FNG / L298N)
* 4 adet IR mesafe sensörü
* LiPo pil (7.4V önerilir)
* Step-down regülatör (5V sabit çıkış)

## Önerilen

* Encoder'lı motor
* IMU (stabilite için)

---

# Labirent Veri Yapısı

Her hücre **1 byte** ile temsil edilir.

```c
uint8_t maze[16][16];
```

## Bit Mask Yapısı

| Bit | Anlam          |
| --- | -------------- |
| 0   | Kuzey duvarı   |
| 1   | Doğu duvarı    |
| 2   | Güney duvarı   |
| 3   | Batı duvarı    |
| 4   | Ziyaret edildi |

## Örnek

```c
maze[x][y] |= 0x01; // kuzey duvarı ekle
maze[x][y] |= 0x10; // visited
```

---

# Flood Fill Algoritması

Amaç: Her hücreye hedefe olan mesafeyi atamak.

## Distance Map

```c
uint8_t dist[16][16];
```

## Başlatma

Hedef hücre(ler) = 0

```c
for all cells:
  dist = INF

dist[target] = 0
```

## Güncelleme (BFS mantığı)

```c
queue.push(target)

while(queue not empty):
  current = queue.pop()

  for each neighbor:
    if no wall:
      if dist[neighbor] > dist[current] + 1:
        dist[neighbor] = dist[current] + 1
        queue.push(neighbor)
```

## Kritik Nokta

* Her **yeni duvar algılandığında** flood fill tekrar çalıştırılmalı

---

# Karar Mekanizması

Robot bulunduğu hücreden:

1. Erişilebilir komşulara bakar
2. **En küçük distance değerini seçer**
3. Eşitse:

   * ziyaret edilmemiş tercih edilir

## Pseudo Code

```c
best = INF
next = NONE

for each direction:
  if no wall:
    if dist[cell] < best:
      best = dist[cell]
      next = direction
```

---

# Hareket Sistemi

## Diferansiyel Sürüş

```text
Sol Motor  |  Sağ Motor
----------------------
Hızlı      | Yavaş     → sağa dön
Yavaş      | Hızlı     → sola dön
Eşit       | Eşit      → düz git
```

## Dönüş Tipleri

* Pivot turn
* Tank turn

---

# PD Kontrol

## Hata

```c
error = leftSensor - rightSensor;
```

## Formül

```c
correction = Kp * error + Kd * (error - lastError);
```

## Motor Güncelleme

```c
leftMotor  = baseSpeed - correction;
rightMotor = baseSpeed + correction;
```

## Tuning

* Kp ↑ → hızlı tepki, overshoot
* Kd ↑ → stabilite, titreşim azaltma

---

# Sensör Okuma

* Analog IR sensör önerilir
* Gürültü filtreleme:

```c
value = (v1 + v2 + v3) / 3;
```

## Threshold

* Ortama göre kalibre edilmeli

---

# Çalışma Modları

## 1. Search Run

* Harita oluşturulur
* Duvarlar işlenir
* Flood fill sürekli güncellenir

Hedefe ulaşınca:

```c
saveToEEPROM();
```

## 2. Fast Run

* Önceden hesaplanmış path kullanılır
* Minimum duraklama
* Maksimum hız

---

# EEPROM Kullanımı

```c
EEPROM.write(address, value);
```

* Maze veya path saklanabilir

---

# Performans Optimizasyonu

* 8-bit integer kullan (uint8_t)
* Recursive kullanma (stack patlar)
* Queue → static array
* Float kullanma (fixed-point tercih et)

---

# Güç Yönetimi

* LiPo pil önerilir
* Regülatör ile 5V sabitlenmeli
* Motor ve MCU ayrı beslenmeli

---

#  Debug Stratejileri

* Serial debug (9600 baud)
* LED state göstergeleri
* Sensör değerlerini logla

---

# Geliştirme Roadmap

## Aşama 1: Donanım

* Motor sür
* Sensör oku

## Aşama 2: Stabil sürüş

* PD kontrol
* Düz git

## Aşama 3: Harita

* Maze + bitmask
* Flood fill

## Aşama 4: Karar

* Path seçimi

## Aşama 5: Optimizasyon

* Hız arttır
* Path smooth

---

# Sonuç

Bu sistem:

* Minimum RAM ile çalışır
* Gerçek zamanlı karar verir
* Yarışma seviyesinde performans hedefler