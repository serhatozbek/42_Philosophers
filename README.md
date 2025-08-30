## 🤖 Philosophers - Bir Senkronizasyon Senfonisi | Ecole 42

Bu repository, 42 müfredatının en temel ve aydınlatıcı projelerinden biri olan philosophers'ın C dilindeki implementasyonunu sunmaktadır. Bu proje, bilgisayar bilimlerinin klasik "Düşünen Filozoflar" problemini, Pthreads (POSIX threads) kütüphanesini kullanarak çözmeyi amaçlar. Bu görev, sadece bir kodlama egzersizi değil, aynı zamanda çoklu iş parçacıklı (multi-threaded) bir ortamda kaynakların nasıl güvenli bir şekilde yönetileceğini, deadlock (kilitlenme) ve data race (veri yarışı) gibi ölümcül hataların nasıl önleneceğini anlamak üzerine kurulu derin bir teknik yolculuktur.

---

#### Projenin Felsefesi ve Kısıtlamaları

Philosophers projesi, bizden sadece bir simülasyon yazmamızı istemedi; eşzamanlılık (concurrency) ve paralellik kavramlarının ruhunu anlamamızı talep etti. Projenin en temel ve zorlayıcı kuralı, tüm bu karmaşık senkronizasyon mantığını yalnızca belirli bir grup harici fonksiyon kullanarak inşa etme zorunluluğuydu. Bu kısıtlama, bizi modern dillerin sunduğu yüksek seviye soyutlamalardan uzaklaştırıp, bir işletim sisteminin thread ve mutex yönetiminin temelleriyle doğrudan çalışmaya itti.

**Tüm proje boyunca kullanmamıza izin verilen fonksiyonlar şunlardı:**

memset, printf, malloc, free, write, usleep, gettimeofday, pthread_create, pthread_detach, pthread_join, pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock

**Bu sınırlı araç setiyle çalışmak, bize şu konularda paha biçilmez yetkinlikler kazandırdı:**

+ **Teknik Hakimiyet:** Thread yaşam döngüsü yönetimi, mutex'lerle kritik bölgelerin korunması, data race'lerin tespiti ve deadlock'u önleyen algoritmik stratejiler geliştirme konularında derin bir anlayış kazandık.

+ **Problem Çözme:** Philosophers, ezberci çözümlerin işe yaramadığı, her bir milisaniyenin ve her bir kilidin kritik öneme sahip olduğu bir projeydi. Karşılaştığımız sayısız "edge case" ve zamanlama senaryosu, algoritmik düşünme ve "debugging" yeteneklerimizi en üst seviyeye taşıdı.

+ **Sistem Düşüncesi:** Tek bir programın içinde, birbirinden bağımsız çalışan onlarca "mini programcığın" (thread'lerin) ortak kaynaklar için nasıl rekabet ettiğini ve bu rekabetin nasıl bir harmoniye dönüştürülebileceğini ilk elden deneyimledik.

+ **Çekirdek Fonksiyonalite:** Teknik Implementasyon

---

#### Projenin geliştirilmesi, 42 tarafından belirlenen katı kurallar çerçevesinde, aşağıdaki mühendislik kararlarıyla şekillendi.

**1. Simülasyonun Kurulumu ve Veri Yapıları**

Gereksinim: Komut satırı argümanlarını (filozof sayısı, yaşam/yemek/uyku süreleri) almak, her filozof ve çatal için gerekli belleği ve mutex'leri oluşturmak.

Çözümüm: Projenin kalbinde, tüm simülasyonun paylaşılan durumunu tutan bir t_data struct'ı ve her bir filozofu temsil eden t_philo struct dizisi bulunur. Program başladığında init.c içindeki fonksiyonlar, tüm bu yapıları malloc ile ayırır, her çatal için bir pthread_mutex_t oluşturur ve her filozofu başlangıç değerleriyle (ID, kendi çatal mutex'lerinin adresleri vb.) hayata hazırlar.

**2. Thread Yönetimi ve Senkron Başlatma**

Gereksinim: Her filozof için bir thread oluşturmak ve tüm filozofların simülasyona tam olarak aynı anda başlamasını garanti altına almak.

Çözümüm: starter.c içerisinde, her bir filozof için pthread_create ile bir thread oluşturulur ve bu thread'e kendi philosopher_routine fonksiyonunu çalıştırması söylenir. Tüm thread'ler oluşturulduktan sonra, start_timer fonksiyonu simülasyonun başlangıç zamanını kaydeder ve tüm thread'lerin aynı anda döngüye başlamasını tetikleyen bir start_flag'i aktif hale getirir. Bu, adil bir başlangıç için hayati önem taşır.

**3. Deadlock Önleme Stratejisi: Asimetrik Çözüm**

Gereksinim: Tüm filozofların aynı anda sol (veya sağ) çatallarını alıp birbirlerini sonsuza dek beklediği deadlock durumunu mutlak surette önlemek.

Çözümüm: Projem, deadlock'u önlemek için kanıtlanmış bir yöntem olan kaynak sıralamasını (resource ordering) kullanır. Tüm filozofların aynı stratejiyi izlemesi yerine, bir asimetri yaratılır:

+ Tek ID'li filozoflar önce sol, sonra sağ çatalı almaya çalışır.

+ Çift ID'li filozoflar ise önce sağ, sonra sol çatalı almaya çalışır.

Bu basit ama etkili strateji, "**dairesel bekleme**" koşulunu imkansız hale getirerek deadlock'u matematiksel olarak engeller.

**4. Gözlemci (Monitor) Thread'i ve Veri Bütünlüğü**

Gereksinim: Bir filozofun açlıktan ölüp ölmediğini veya tüm filozofların yemesi gereken minimum yemeği yiyip yemediğini sürekli olarak denetlemek. Bu denetim sırasında data race oluşmasını engellemek.

Çözümüm: Ana filozof thread'lerinin yanı sıra, bir gözlemci thread'i (monitor_routine) oluşturulur. Bu "hakem" thread, sürekli olarak tüm filozofları kontrol eder. Bir filozofun son yemeğinden bu yana geçen sürenin time_to_die'ı aşıp aşmadığını kontrol ederken, last_meal_time gibi paylaşılan değişkenlere erişim data_mutex ile korunur. Bu, hem filozofun bu değeri yazması hem de monitörün okuması sırasında oluşabilecek data race'leri engeller.

---

### ⚙️ Kullanım Talimatları

**Derleme**

```Bash

make
```
**Çalıştırma**

```Bash

#./philo [filozof_sayısı] [ölme_süresi_ms] [yemek_süresi_ms] [uyku_süresi_ms] [yemek_sayısı (opsiyonel)]
#Örnek:

./philo 5 800 200 200 7

```


#### Eşzamanlılık Hatalarını Test Etme (Data Race & Deadlock)

Philosophers projesinin en kritik gereksinimi, hiçbir koşulda data race veya deadlock içermemesidir. Bu tür hatalar, normal testlerde her zaman ortaya çıkmayabilir. Bu nedenle, programı daha agresif yöntemlerle test etmek hayati önem taşır.

#### Data Race Tespiti (ThreadSanitizer)

Projenin en sinsi hatalarından olan data race'leri tespit etmek için, derleyiciye özel bir bayrak ekleyerek ThreadSanitizer (TSan) aracını kullanmak en iyi yöntemdir. Bu araç, paylaşılan bir veriye iki thread'in aynı anda ve korumasız bir şekilde eriştiği anları tespit eder.


```Bash

# -fsanitize=thread bayrağı ile derleme
# makefile içinde flags kısmına ekleyin : "-fsanitize=thread"

# Testi çalıştırma
./philo 4 410 200 200

```
**Eğer fsanitize çalışırken herhangi bir çıktı vermezse, bu programınızda belirgin bir data race olmadığını gösterir. Bir hata bulursa, detaylı bir rapor sunacaktır.**

---

**Bellek Yönetimi ve Sızıntı Kontrolü (Valgrind)**

Programın hiçbir bellek sızıntısı (leak) yapmadığından emin olmak için Valgrind kullanılır.

```Bash

valgrind --leak-check=full ./philo 5 800 200 200 7
```
**"All heap blocks were freed -- no leaks are possible" çıktısını almak, projenin bellek yönetimindeki başarısını gösterir.**

---

### ⚠️ Sorumluluk Reddi

Bu repository'deki çözümler tamamen eğitim ve portföy amaçlıdır. 42'nin onur kuralları (honour code) gereği, Common Core sürecindeki öğrencilerin çözümleri kopyalaması kesinlikle yasaktır. Buradaki kodlar, repodaki projeyi tamamlamış biri olarak gelişimimi göstermek ve gelecekteki projelerim için bir referans noktası oluşturmak amacıyla paylaşılmıştır.

---

