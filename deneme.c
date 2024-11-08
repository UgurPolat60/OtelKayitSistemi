#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char isim[50];
    char soyisim[50];
    char kullaniciAdi[50];
    char sifre[50];
} Kisi;

typedef struct {
    int girisAy;
    int cikisAy;
    int toplamFiyat;
    char kullanan[50];
} Gecmis;

typedef struct {
    char odaTipi[50];
    int fiyat;
    int dolu;
    int girisAy;
    int cikisAy;
    char kullanan[50];
    Gecmis gecmisKayitlar[100];
    int gecmisSayisi;
} Oda;

Kisi *kisiler = NULL;
int kisiSayisi = 0;
Kisi *aktifKullanici = NULL;

Oda odalar[6] = {
    {"Premium Tek", 2000, 0, 0, 0, ""},
    {"Premium Çift", 3000, 0, 0, 0, ""},
    {"Normal Tek", 1300, 0, 0, 0, ""},
    {"Normal Çift", 2000, 0, 0, 0, ""},
    {"Her Şey Hariç Tek", 800, 0, 0, 0, ""},
    {"Her Şey Hariç Çift", 1300, 0, 0, 0, ""}
};

int mevcutTarih = 1;






void TarihGuncelle() {
     FILE *dosya = fopen("*dosya konumu*", "a"); 
      fprintf(dosya, "Yönetici Tarihi: %d\n", mevcutTarih);    
}

void dosyayaKullaniciKaydet() {
    FILE *dosya = fopen("C:\\Users\\Acer\\Desktop\\Kullanicilar.txt", "a"); 
    if (dosya) {
        
        for (int i = 0; i < kisiSayisi; i++) {
            fprintf(dosya, "Kullanıcı: %s %s, Kullanıcı Adı: %s, Şifre: %s\n",
                    kisiler[i].isim, kisiler[i].soyisim, kisiler[i].kullaniciAdi, kisiler[i].sifre);
        }

        fclose(dosya);
    } else {
        printf("Kullanıcı dosyası açılamadı!\n");
    }
}

void dosyayaGecmisKaydet() {
    FILE *dosya = fopen("C:\\Users\\Acer\\Desktop\\otelgecmisi.txt", "a"); 
    if (dosya) {
       

        for (int i = 0; i < 6; i++) {
            if (odalar[i].dolu) { 
                for (int j = 0; j < odalar[i].gecmisSayisi; j++) {
                    fprintf(dosya, "%s odası, %s, Giriş: %d, Çıkış: %d, Ödenen Fiyat: %d TL\n",
                            odalar[i].odaTipi, odalar[i].gecmisKayitlar[j].kullanan,
                            odalar[i].gecmisKayitlar[j].girisAy, odalar[i].gecmisKayitlar[j].cikisAy,
                            odalar[i].gecmisKayitlar[j].toplamFiyat);
                }
            }
        }

        fclose(dosya);
    } else {
        printf("Geçmiş dosyası açılamadı!\n");
    }
}




int girisYap() {
    char kullaniciAdi[50];
    char sifre[50];

    printf("Kullanıcı Adı: ");
    scanf("%s", kullaniciAdi);
    printf("Şifre: ");
    scanf("%s", sifre);

    if (strcmp(kullaniciAdi, "Ugur") == 0 && strcmp(sifre, "Ugurluck") == 0) {
        printf("Yönetici girişi yapıldı! Hoş geldiniz, Uğur Polat.\n");
        aktifKullanici = NULL;
        return 2;
    }

    for (int i = 0; i < kisiSayisi; i++) {
        if (strcmp(kisiler[i].kullaniciAdi, kullaniciAdi) == 0 &&
            strcmp(kisiler[i].sifre, sifre) == 0) {
            printf("Giriş yapıldı! Hoş geldiniz, %s %s.\n", kisiler[i].isim, kisiler[i].soyisim);
            aktifKullanici = &kisiler[i];
            return 1;
        }
    }

    printf("Kullanıcı adı veya şifre yanlış!\n");
    return 0;
}

void odaDurumunuGoster() {
    printf("\nOda Seçenekleri:\n");
    for (int i = 0; i < 6; i++) {
        printf("%d. %s - Fiyat: %d TL ", i + 1, odalar[i].odaTipi, odalar[i].fiyat);
        
        if (odalar[i].dolu) {
            printf("(Dolu: ");
            for (int j = 0; j < odalar[i].gecmisSayisi; j++) {
                // Her doluluk aralığını göstermek için, giriş ve çıkış aylarını yazdırıyoruz
                printf("%d - %d", odalar[i].gecmisKayitlar[j].girisAy, odalar[i].gecmisKayitlar[j].cikisAy);
                if (j < odalar[i].gecmisSayisi - 1) {
                    printf(", "); // Son elemandan sonra virgül koymayalım
                }
            }
            printf(")\n");
        } else {
            printf("(Dolu: Hayır)\n");
        }
    }
}


int odaMüsaitMi(int odaIndex, int giris, int cikis) {
    if (odalar[odaIndex].dolu) {
        for (int j = 0; j < odalar[odaIndex].gecmisSayisi; j++) {
            if (giris < odalar[odaIndex].gecmisKayitlar[j].cikisAy && 
                cikis > odalar[odaIndex].gecmisKayitlar[j].girisAy) {
                return 0; 
            }
        }
    }
    return 1; 
}


void odaSeçimi() {
    int odaSecim, gunSayisi, girisAy, cikisAy;
    
    if (aktifKullanici == NULL) {
        printf("Lütfen geçerli bir kullanıcı girişi yapın.\n");
        return;
    }

    odaDurumunuGoster();

    printf("Hangi odayı seçmek istersiniz? (1-6): ");
    scanf("%d", &odaSecim);
    odaSecim--;

    printf("Ayın kaçında giriş yapacaksınız? (1-31): ");
    scanf("%d", &girisAy);
    printf("Ayın kaçında çıkacaksınız? (1-31): ");
    scanf("%d", &cikisAy);

    gunSayisi = cikisAy - girisAy;
    if (gunSayisi <= 0) {
        printf("Çıkış tarihi giriş tarihinden önce olamaz!\n");
        return;
    }

    if (!odaMüsaitMi(odaSecim, girisAy, cikisAy)) {
        printf("Seçilen tarihlerde bu oda doludur! Rezervasyon yapılamaz.\n");
        return;
    }

    int toplamFiyat = odalar[odaSecim].fiyat * gunSayisi;
    printf("Seçilen Oda: %s, Giriş: %d, Çıkış: %d, Gün Sayısı: %d, Toplam Fiyat: %d TL\n", 
        odalar[odaSecim].odaTipi, girisAy, cikisAy, gunSayisi, toplamFiyat);
    
    char onay;
    printf("Almak istiyor musunuz? (E/H): ");
    scanf(" %c", &onay);

    if (onay == 'E' || onay == 'e') {
        odalar[odaSecim].dolu = 1;
        odalar[odaSecim].girisAy = girisAy;
        odalar[odaSecim].cikisAy = cikisAy;
        
        Gecmis *gecmis = &odalar[odaSecim].gecmisKayitlar[odalar[odaSecim].gecmisSayisi++];
        gecmis->girisAy = girisAy;
        gecmis->cikisAy = cikisAy;
        gecmis->toplamFiyat = toplamFiyat;
        strcpy(gecmis->kullanan, aktifKullanici->isim);

        printf("Rezervasyon başarılı! Odanız ayrılmıştır.\n");

        
dosyayaGecmisKaydet();
    } else {
        printf("Rezervasyon iptal edildi.\n");
    }
}

void tarihiYaz() {
    printf("Yeni tarihi yazın: ");
    scanf("%d", &mevcutTarih);
    TarihGuncelle();
    printf("Tarih güncellendi.\n");

    for (int i = 0; i < 6; i++) {
        if (odalar[i].dolu && mevcutTarih >= odalar[i].cikisAy) {
            odalar[i].dolu = 0;
            odalar[i].girisAy = 0;
            odalar[i].cikisAy = 0;
            strcpy(odalar[i].kullanan, "");
        }
    }

}

void yoneticiIslemleri() {
    while (1) {
        printf("\nYönetici İşlemleri:\n");
        printf("1. Tarih Yaz\n");
        printf("2. Çıkış\n");
        int secim;
        printf("Seçim: ");
        scanf("%d", &secim);

        if (secim == 1) {
            tarihiYaz();
        } else if (secim == 2) {
            break;
        } else {
            printf("Geçersiz seçim!\n");
        }
    }
}

void kayitOl() {
    Kisi yeniKisi;
    printf("İsim: ");
    scanf("%s", yeniKisi.isim);
    printf("Soyisim: ");
    scanf("%s", yeniKisi.soyisim);
    printf("Kullanıcı Adı: ");
    scanf("%s", yeniKisi.kullaniciAdi);
    printf("Şifre: ");
    scanf("%s", yeniKisi.sifre);

    kisiler = realloc(kisiler, sizeof(Kisi) * (kisiSayisi + 1));
    kisiler[kisiSayisi++] = yeniKisi;
    printf("Kayıt başarılı!\n");

dosyayaKullaniciKaydet();
}

int main() {
    
      
    while (1) {
        printf("\n1. Giriş Yap\n2. Kayıt Ol\n3. Çıkış\n");
        int secim;
        printf("Seçiminizi yapın: ");
        scanf("%d", &secim);

        if (secim == 1) {
            int durum = girisYap();
            if (durum == 2) {
                yoneticiIslemleri();
            } else if (durum == 1) {
                odaSeçimi();
            }
        } else if (secim == 2) {
            kayitOl();
        } else if (secim == 3) {
            printf("Çıkılıyor...\n");
            break;
        } else {
            printf("Geçersiz seçim!\n");
        }
    }

    free(kisiler);
    return 0;
}

