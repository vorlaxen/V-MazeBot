# Faz 4: 2. Tur (Hız Koşusu - Fast Run)

Robotun harita çıkarmaya veya karar vermeye ihtiyacı kalmamıştır. Elinde optimize edilmiş bir komut listesi vardır ve tek amacı bunu fiziksel sınırlarını zorlayarak olabildiğince hızlı uygulamaktır.

## Yapılan Geliştirmeler

### 1. Komut İşleyici (Command Executor)
- `loop()` fonksiyonunun içine `yolHesaplandi` koşulundan sonra devreye giren bir komut işleyici eklenmiştir.
- Robot sırasıyla dizideki elemanları okur. 'R' gelirse 90 derece sağa, 'L' gelirse 90 derece sola döner.
- Sayı (Örn: '4') gelirse, hesaplanan süre kadar yüksek hızda dümdüz ilerler.
- Tanımlanmayan bir komut gelirse atlayarak sonsuz döngüye girmesi engellenmiştir.

### 2. Agresif PID Kontrolü
Hız koşusu için temel sürüş hızında ve PID çarpanlarında ciddi artışa gidilmiştir.
- İlk turdaki yavaş, etrafı anlamaya çalışan PID parametreleri (`Kp`, `Kd`, `Ki`) ile, hızlı koşu için hazırlanan parametreler (`Kp_hizli`, `Kd_hizli`) değiştirilmiştir.
- Hız arttıkça salınım ihtimali artacağından `Kd` (Türev) değeri artırılarak robotun düz yolda çok daha sert ve stabil bir şekilde çizgiyi (veya duvar aralığını) koruması hedeflenmiştir.
- Motor hızı `HIZ_hizli` olarak güncellenmiş ve hücre geçiş süreleri bu yüksek hıza göre orantılanmıştır.

## Özet
Faz 4, robotun bir "Haritalayıcı" olmaktan çıkıp tam bir "Yarışçı" olduğu aşamadır. Sadece hafızasındaki yolu uygular, duvara çarpmamak için agresif PID kullanır ve hedefe ulaştığında durur.
