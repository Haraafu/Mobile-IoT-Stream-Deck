# Mobile Stream Deck (ESP32 + Blynk + Python Agent)

Proyek **Mobile Stream Deck** ini memungkinkan pengguna mengontrol aplikasi di komputer menggunakan smartphone melalui aplikasi **Blynk IoT**. Dengan menekan tombol virtual di Blynk, perintah dikirim ke **ESP32**, diteruskan ke **Blynk Cloud**, dan dijalankan oleh **Python agent** di PC untuk membuka aplikasi seperti **Spotify** atau **YouTube** secara otomatis. Sistem ini bekerja layaknya *stream deck* sederhana berbasis IoT tanpa membutuhkan tombol fisik.

---

## Fitur Utama
- Kontrol aplikasi PC menggunakan smartphone melalui **Blynk IoT App**.  
- **V0 → Buka Spotify**, **V1 → Buka YouTube** secara otomatis.  
- Status aktivitas ditampilkan pada **Label Value V4**.  
- Komunikasi real-time via **Blynk Cloud API**.  
- Tanpa komponen hardware tambahan selain ESP32.  
- Mudah dikembangkan untuk kontrol aplikasi lain seperti VSCode, Notepad, Steam, dll.

---

## Arsitektur Sistem


Penjelasan:
- **Blynk App**: Antarmuka pengguna berupa tombol Toggle.  
- **Blynk Cloud**: Perantara komunikasi antara device dan PC.  
- **ESP32**: Menghubungkan perangkat ke cloud menggunakan WiFi.  
- **Python Agent**: Program di PC yang membaca nilai V-pin dan mengeksekusi aplikasi.

---

## Komponen
| Komponen                | Deskripsi                                           |
|------------------------|-----------------------------------------------------|
| ESP32                  | Mikrokontroler utama yang terhubung ke internet    |
| Blynk Mobile App       | UI kontrol berbasis cloud                           |
| Laptop/PC              | Menjalankan Python Agent                            |
| Koneksi Internet       | Sinkronisasi ESP32 ↔ Cloud ↔ PC                     |

---

## Setup

### 1. Konfigurasi Blynk
Di aplikasi **Blynk IoT**:
- Tambahkan **Switch V0** → Spotify (Mode: Toggle)  
- Tambahkan **Switch V1** → YouTube (Mode: Toggle)  
- Tambahkan **Value Display V4** → Status  
- Ambil **Blynk Auth Token**

---

### 2. Upload Kode ESP32

```cpp
#define BLYNK_TEMPLATE_ID "PASTE_NEW_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "PASTE_NEW_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "PASTE_NEW_TOKEN_HERE"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

const char* WIFI_SSID = "YOUR_WIFI";
const char* WIFI_PASS = "YOUR_PASSWORD";

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
}

void loop() {
  Blynk.run();
}
```

### 3. Install & Run
python -m venv venv
venv\Scripts\activate
pip install requests
python StreamDeck.py
