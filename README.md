# Ninja Saga AlPro 🥷✨

**Version:** `1.0.0-beta`

Selamat datang di **Ninja Saga AlPro**, sebuah game *turn-based RPG* bergaya retro 2D pixel-art yang dikembangkan menggunakan **C++** dan **Raylib**. Game ini merupakan proyek akhir Algoritma Pemrograman yang memadukan logika fundamental pemrograman dengan antarmuka grafis interaktif.

## 🌟 Fitur Utama (Game Features)

- **Interactive UI & Splash Screen:** Layar pembuka (splash screen) dengan gaya teks *arcade retro*, dan Main Menu interaktif berbasis deteksi posisi kursor (*hover*).
- **Sistem Pertarungan Turn-Based (Battle System):** Bertarung melawan musuh (Demon) menggunakan sistem giliran. Atur strategi serangan fisik maupun penggunaan sihir (*Skills*) dengan memperhatikan *Health Points (HP)* dan *Mana*. Dilengkapi animasi serangan dan proyektil.
- **Ninja Academy (Skill Shop):** Sistem *shop* tempat pemain dapat menukarkan *Gold* yang didapat dari pertarungan untuk mempelajari kemampuan baru (seperti *Fire*, *Ice*, atau *Heal*). Sistem ini otomatis mendeteksi jika pemain sudah memiliki skill tersebut.
- **Inventory System:** Sistem penyimpanan barang/kemampuan bergaya grid 3x3 yang interaktif, menampilkan *hover indicator* yang menyesuaikan pergerakan *mouse* secara real-time.
- **Sistem Auto-Save (File Persistence):** Progres pemain (termasuk *Level*, *Gold*, *EXP*, atribut dasar, dan daftar *skill* yang telah dibeli) akan secara otomatis tersimpan ke dalam file lokal (`savegame.txt`) saat keluar dari game, dan otomatis diload ketika game dimainkan kembali tanpa bug duplikasi.

## 🛠️ Tech Stack & Requirements

- **Bahasa Pemrograman:** C++17 (atau lebih baru)
- **Library Grafis:** Raylib 6.0
- **Build System:** CMake & Make

## 🚀 Cara Menjalankan (How to Run)

Pastikan Anda telah menginstal compiler C++ (seperti `MinGW/GCC` atau `MSVC` untuk Windows, `g++` untuk Linux) dan `CMake`.

### 🪟 Windows & 🐧 Linux

Buka terminal (atau Command Prompt / PowerShell) pada direktori *root* proyek ini, lalu jalankan perintah berikut:

1. **Konfigurasi CMake:**
   ```bash
   cmake -S . -B build
   ```
2. **Compile Game:**
   ```bash
   cmake --build build
   ```

3. **Jalankan Game:**
   - **Windows:**
     ```cmd
     .\build\bin\NinjaSagaAlPro.exe
     ```
     *(Catatan: Lokasi executable mungkin berada di dalam `.\build\Debug\` atau `.\build\Release\` tergantung konfigurasi generator CMake).*
   - **Linux:**
     ```bash
     ./build/bin/NinjaSagaAlPro
     ```

## 🎮 Cara Bermain (Controls)

- **Mouse / Left Click:** Navigasi antarmuka UI (Main Menu, Inventory, Academy).
- **Keyboard / Mouse (Battle):** Gunakan input kursor atau tombol navigasi yang disediakan di layar (bergantung pada implementasi interaksi UI yang ada) untuk memilih antara Attack, Skill, atau aksi lainnya.

---
