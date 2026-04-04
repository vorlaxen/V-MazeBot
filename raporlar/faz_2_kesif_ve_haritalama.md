# Faz 2: 1. Tur (Keşif ve Haritalama - Search Phase)

Birinci turda robotun asıl amacı labirenti en kısa sürede çözmek değil, labirenti dolaşarak merkezdeki hedefe giden yolu güvenilir bir şekilde haritalandırmaktır.

## Yapılan Geliştirmeler

### 1. Sanal Harita (`labirent[][]`) ve Mutlak Yönler
Labirent 16x16 boyutunda bir `byte` dizisi ile temsil edilmektedir. Her hücre, içinde 4 duvarın (Kuzey, Doğu, Güney, Batı) varlık/yokluk durumunu bitler halinde tutar.
- **Sensör Verisi İşleme (`Duvarlari_Haritaya_Isle()`):** Sensörler sadece robotun sağı, solu ve önünü okur. Fakat haritaya bu duvarlar "robotun baktığı yöne" göre çevrilerek kaydedilir. Örneğin, robot Doğu'ya bakıyorsa ve solunda duvar varsa, bu aslında Kuzey duvarıdır.
- **Komşu Duvarlar:** Eğer (X,Y) hücresinin Kuzeyinde duvar varsa, mantıksal olarak (X,Y+1) hücresinin de Güneyinde duvar olmak zorundadır. Bu optimizasyon haritaya dahil edilmiştir.

### 2. Flood Fill (Sel Dolgusu) Algoritması
Hedef noktasına (merkez 4 hücreye) ulaşmanın maliyetini (uzaklığını) hesaplayan yapay zeka algoritmasıdır.
- **Nasıl Çalışır:** Merkezin mesafesi 0 kabul edilir. Merkezden başlanarak, haritada duvar olmayan tüm komşu hücrelere dalga dalga yayılarak (BFS - Breadth-First Search) mesafeler 1, 2, 3... şeklinde yazılır.
- **Dinamik Güncelleme:** Robot her yeni hücreye girdiğinde ve yeni bir duvar keşfettiğinde Flood Fill algoritması tekrar çalışır. Böylece robot her adımda en mantıklı (mesafesi en düşük) yöne sapar.

### 3. Merkeze Ulaşma ve Karar Alma (`Bir_Sonraki_Gidilecek_Yon()`)
Robotun hangi yöne gideceği tamamen Flood Fill haritasına bağlıdır. Bulunduğu hücrenin etrafındaki (ve duvarla kapalı olmayan) komşu hücrelere bakar, mesafesi (flood_mesafe) en küçük olan hücreye yönlenir.
- Robot labirentin merkezi olan (7,7), (7,8), (8,7), (8,8) noktalarından birine ulaştığında `kesifBitti` bayrağı aktifleşir ve keşif aşaması sonlanır.

## Özet
Bu aşamada robot bir kör gibi deneme yanılma yapmak yerine, kendi içindeki sanal haritayı sürekli güncelleyerek merkeze giden dinamik ve akıllı bir rota çizer.
