# Faz 3: İki Tur Arası (Yol Optimizasyonu)

Robot labirentin merkezini bulduğunda (Keşif bittiğinde), harita çıkarmak için girdiği çıkmaz sokakları ve gereksiz manevraları silmeli, başlangıçtan hedefe giden pürüzsüz ve en kısa yolu çıkarmalıdır.

## Yapılan Geliştirmeler

### 1. En Kısa Yolu Çıkartma (`Yolu_Hesapla()`)
Robot merkeze ulaştığında, 16x16'lık alanın duvar haritası ve Flood Fill mesafeleri hazırdır.
- Başlangıç noktası olan (0,0)'dan başlayarak tıpkı robotun yaptığı gibi en küçük mesafe (flood_mesafe) takip edilir.
- Bu kez robot hareket etmez; sanal olarak bu yolu izleyerek dönüş (Sağ 'R', Sol 'L') ve ileri ('F') gitme komutlarını oluşturur.
- Çıkmaz sokaklara giriş ve çıkışlar haritada engelli olacağı veya Flood Fill mesafesini uzatacağı için bu hesaplamada doğrudan dışarıda kalır.

### 2. Hareketleri Birleştirme ve Optimizasyon
Klasik "İleri, İleri, İleri" komutları tek tek hücre olarak işlenirse robot her hücrede gereksiz yere dur/kalk yapar.
- **Dizi Optimizasyonu:** Peş peşe gelen 'F' (İleri) komutları taranır ve birleştirilir. Örneğin 5 kere 'F' varsa, bu "5 hücre boyunca durmadan ileri git" şeklinde tek bir komuta ('5') dönüştürülür.
- **SRAM Optimizasyonu:** `Yolu_Hesapla` fonksiyonundaki `while` döngüsü içinde, sadece 9'a kadar ('1'-'9') hücre sayısının birleşmesine izin verilmiştir. Bu sayede ASCII karakter taşması engellenmiş olur.

## Özet
Bu aşama, robotun keşif verilerini derleyerek yüksek hızlı 2. tura hazırlık yaptığı köprü aşamasıdır. Karışık hareketler silinmiş, düzlükler birleştirilmiş, hızlanmaya uygun temiz bir rota listesi (`komutlar` dizisi) elde edilmiştir.
