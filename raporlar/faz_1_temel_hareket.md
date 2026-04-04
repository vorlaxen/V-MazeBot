# Faz 1: Temel Hareket ve Kontrol Güncellemeleri

Bu fazın amacı, robotun fiziksel dünyada hassas ve istikrarlı hareket edebilmesini sağlamaktır. Klasik bir çizgi izleyen veya engelden kaçan robottan farklı olarak, labirent çözen robotların uzaysal bir farkındalığı olmalıdır. Labirent, belirli ölçülerde (örneğin 18x18 cm) hücrelerden oluşur ve robot bu hücreler arasında bir satranç taşı gibi hareket etmelidir.

## Yapılan Geliştirmeler

### 1. Hücre Bazlı İlerleme (`Bir_Hucre_Ileri()`)
Sürekli ileri gitmek yerine, robot tam bir hücre mesafesi kadar ilerler ve ardından durup sensör verisi okumaya hazır hale gelir.
- **Nasıl Çalışır:** Zaman ve PID kontrolü birleştirilmiştir. PID döngüsü içinde robot düz gitmeye çalışırken, belirtilen süre (`hucreGecisSuresi`) dolana kadar ilerler ve durur. (Gelecekte enkoder entegrasyonu ile süre yerine kat edilen mesafe sayılabilir).
- **Konum Güncellemesi:** Hareket tamamlandıktan sonra, robotun o an baktığı yöne (`robotYon`) göre X veya Y koordinatı (`robotX`, `robotY`) güncellenir. Böylece robot haritada tam olarak nerede olduğunu bilir.

### 2. Hassas 90 Derecelik Dönüşler (`Saga_Don_90()`, `Sola_Don_90()`, `Donus_180()`)
Dönüşler artık anlık olarak sensör eşiklerine değil, tam 90 derece olacak şekilde zamanlı/açısal dönüşlere bağlıdır.
- **Nasıl Çalışır:** Belirli bir hızda, tekerlekler birbirine zıt yönlerde çevrilerek nokta dönüşü yapılır. Süre ayarı ile tam 90 derece sağlanır.
- **Yön Güncellemesi:** Dönüş tamamlandığında `robotYon` değişkeni modüler aritmetik kullanılarak güncellenir (Örn: Kuzeye bakarken sağa dönerse Doğuya bakar `(0 + 1) % 4 = 1`).

## Özet
Bu aşama sayesinde robotun rastgele dolaşması engellenmiş, haritalama algoritmasının temelini atacak kesin ve "hücre hücre" hareket kabiliyeti kazandırılmıştır. Her hareketin sonunda koordinat ve yön bilgisi güncel tutulmaktadır.
