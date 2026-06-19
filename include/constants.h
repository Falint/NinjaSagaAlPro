#pragma once

//===============================================
// -- Splash Scene Constants --
//===============================================
constexpr float SPLASH_FADE_IN_DURATION = 1.0f;
constexpr float SPLASH_HOLD_DURATION = 2.0f;
constexpr float SPLASH_FADE_OUT_DURATION = 1.0f;
constexpr float SPLASH_TOTAL_DURATION =
    SPLASH_FADE_IN_DURATION + SPLASH_HOLD_DURATION + SPLASH_FADE_OUT_DURATION;
constexpr int SPLASH_TITLE_FONT_SIZE = 80;


//===============================================
// -- Main Menu Scene Constants --
//===============================================
// Asumsi resolusi asli pixel art menu adalah 200x200
constexpr float MENU_BASE_SIZE = 200.0f;

// Path untuk 6 frame Menu
// Asumsi frame 1 adalah Normal, sisanya adalah state ketika di-hover/diklik
constexpr const char *ASSET_MENU_FRAME_1 = "assets/sprites/ui/mainmenu1.png";
constexpr const char *ASSET_MENU_FRAME_2 = "assets/sprites/ui/mainmenu2.png";
constexpr const char *ASSET_MENU_FRAME_3 = "assets/sprites/ui/mainmenu3.png";
constexpr const char *ASSET_MENU_FRAME_4 = "assets/sprites/ui/mainmenu4.png";
constexpr const char *ASSET_MENU_FRAME_5 = "assets/sprites/ui/mainmenu5.png";
constexpr const char *ASSET_MENU_FRAME_6 = "assets/sprites/ui/mainmenu6.png";

// Hitbox tombol (dalam rasio 0.0 - 1.0 relatif terhadap gambar 200x200)
// Anda bisa menyesuaikan koordinat ini dengan letak teks di LibreSprite Anda
constexpr int BUTTON_COUNT = 5;
// Format: { X_Ratio, Y_Ratio, Width_Ratio, Height_Ratio }
// Hitbox dibuat lebih ke atas dan ukurannya lebih kecil (height 0.05, width
// 0.3)
constexpr float MENU_HITBOXES[BUTTON_COUNT][4] = {
    {0.30f, 0.27f, 0.3f, 0.05f}, // PLAY
    {0.30f, 0.35f, 0.3f, 0.05f}, // SHOP
    {0.30f, 0.44f, 0.3f, 0.05f}, // INVENTORY
    {0.30f, 0.53f, 0.3f, 0.05f}, // STATUS
    {0.30f, 0.61f, 0.3f, 0.05f}, // EXIT
};

//===============================================
// -- Inventory Scene Constants --
//===============================================
constexpr const char *ASSET_INVENTORY_FRAME = "assets/sprites/ui/Inv.png";
constexpr const char *ASSET_INVENTORY_HITBOX =
    "assets/sprites/ui/Inv_hitbox.png";
constexpr const char *ASSET_CHARACTER_IDLE = "assets/sprites/ui/idle_down.png";
constexpr const char *ASSET_BTN_KEMBALI = "assets/sprites/ui/button/btn back.png";

// Offset dan Scale untuk karakter animasi di UI Inventory
constexpr float INV_CHAR_SCALE =
    1.5f; // Pengali ukuran karakter (semakin besar makin raksasa)
constexpr float INV_CHAR_OFFSET_X =
    0.0f; // Jarak geser horizontal (0 = tepat di tengah)
constexpr float INV_CHAR_OFFSET_Y =
    -90.0f; // Jarak karakter dari atas UI inventory

// gampang kalibrasi posisi
constexpr bool INV_CHAR_DEBUG_HITBOX = false;

// Ukuran asli texture Inv.png
constexpr float INV_BASE_SIZE = 240.0f;

// Scale factor untuk mengecilkan/membesarkan inventory frame
// Ubah nilai ini untuk mengontrol ukuran: 0.3 = kecil, 0.6 = sedang, 1.0 =
// besar
constexpr float INV_SCALE_FACTOR = 0.6f;

// Grid layout
constexpr int INV_GRID_COLS = 3;
constexpr int INV_GRID_ROWS = 3;
constexpr int INV_SLOT_COUNT = INV_GRID_COLS * INV_GRID_ROWS; // 9

// Debug flags per-scene (set true untuk melihat kotak hitbox berwarna)
constexpr bool INV_DEBUG_HITBOX = false;
constexpr bool MENU_DEBUG_HITBOX = false;

// Per-slot hitbox (rasio relatif terhadap cell size masing-masing)
// Dari analisis Inv_hitbox.png (64x64): inner area pixel (18,18)→(45,47)
// Format: { X_Ratio, Y_Ratio, Width_Ratio, Height_Ratio }
// Setiap slot bisa diubah sendiri tanpa mempengaruhi slot lain
constexpr float INV_SLOT_HITBOXES[INV_SLOT_COUNT][4] = {
    {0.27f, 0.25f, 0.74f, 0.78f}, // Slot 0 (kiri atas)
    {0.13f, 0.25f, 0.74f, 0.78f}, // Slot 1 (tengah atas)
    {-0.01f, 0.25f, 0.74f, 0.78f}, // Slot 2 (kanan atas)
    {0.27f, 0.12f, 0.74f, 0.77f}, // Slot 3 (kiri tengah)
    {0.13f, 0.12f, 0.74f, 0.77f}, // Slot 4 (tengah)
    {-0.01f, 0.12f, 0.74f, 0.77f}, // Slot 5 (kanan tengah)
    {0.27f, -0.03f, 0.74f, 0.77f}, // Slot 6 (kiri bawah)
    {0.13f, -0.03f, 0.74f, 0.77f}, // Slot 7 (tengah bawah)
    {-0.01f, -0.03f, 0.74f, 0.77f}, // Slot 8 (kanan bawah)
};

// Ukuran dan posisi gambar tombol kembali
constexpr float INV_BTN_KEMBALI_VISUAL[4] = {-0.25f, 1.0f, 0.50f, 0.22f};
// Ukuran Hitbox tombol kembali 
constexpr float INV_BTN_KEMBALI_HITBOX[4] = {-0.206f, 1.065f, 0.4f, 0.09f,};
constexpr bool INV_BTN_KEMBALI_DEBUG_HITBOX = false; // Untuk Hitbox dan Gambar 

//===============================================
// -- Battle Scene Constants --
//===============================================

// Assets Main Menu Background
// -- Main Menu Scene Constants --
constexpr const char *ASSET_MENU_BACKGROUND = "assets/sprites/ui/main_menuBackground.png";
constexpr const char *ASSET_MENU_BGM = "assets/audio/soundtrack/MiniMacro Sound - The Ninja Gaiden Soundtrack as Interpreted by MiniMacro Sound - 04 Vow of Revenge (Opening II).mp3";


// Assets Battle Background
constexpr const char *ASSET_BATTLE_BACKGROUND = "assets/maps/battle_bkg.png";

// Assets Player
constexpr const char *ASSET_BATTLE_PLAYER_IDLE =
    "assets/sprites/ui/Player/man/idle.png";
constexpr const char *ASSET_BATTLE_PLAYER_ATTACK =
    "assets/sprites/ui/Player/man/attack.png";
// Assets Enemy (Demon)
constexpr const char *ASSET_BATTLE_ENEMY_IDLE =
    "assets/sprites/ui/Enemy/Demon/IDLE.png";
constexpr const char *ASSET_BATTLE_ENEMY_ATTACK =
    "assets/sprites/ui/Enemy/Demon/ATTACK.png";
constexpr const char *ASSET_BATTLE_ENEMY_HURT =
    "assets/sprites/ui/Enemy/Demon/HURT.png";
constexpr const char *ASSET_BATTLE_ENEMY_DEATH =
    "assets/sprites/ui/Enemy/Demon/DEATH.png";
constexpr const char *ASSET_BATTLE_ENEMY_PROJECTILE =
    "assets/sprites/ui/Enemy/Demon/projectile.png";

// Assets Health Bar
constexpr const char *ASSET_BATTLE_HEALTH_BG =
    "assets/sprites/ui/bar/health/emptyCellMeter.png";
// Kita simpan prefix dan max untuk looping nama file: redMeter01.png -
// redMeter10.png
constexpr const char *ASSET_BATTLE_HEALTH_FILL_PREFIX =
    "assets/sprites/ui/bar/health/redMeter";

// Battle Stats
constexpr int BATTLE_PLAYER_MAX_HP = 10;
constexpr int BATTLE_PLAYER_DAMAGE = 2;
constexpr int BATTLE_ENEMY_MAX_HP = 10;
constexpr int BATTLE_ENEMY_DAMAGE = 1;

// Rendering Scales & Positions
constexpr float BATTLE_PLAYER_SCALE = 3.0f;
constexpr float BATTLE_ENEMY_SCALE = 3.0f;
constexpr float BATTLE_PROJECTILE_SCALE = 2.0f;

// Frame counts dari analisis
constexpr int BATTLE_PLAYER_FRAMES_IDLE = 8;
constexpr int BATTLE_PLAYER_FRAMES_ATTACK = 8;
constexpr int BATTLE_ENEMY_FRAMES_IDLE = 4;
constexpr int BATTLE_ENEMY_FRAMES_ATTACK = 8;
constexpr int BATTLE_ENEMY_FRAMES_HURT = 4;
constexpr int BATTLE_ENEMY_FRAMES_DEATH = 7; // Menggunakan frame 79px

// Animation FPS
constexpr int BATTLE_ANIM_FPS = 8;

// Timers
constexpr float BATTLE_STATE_DELAY = 2.0f; // Delay per turn state
constexpr float BATTLE_END_DELAY =
    3.0f; // Delay saat win/lose sebelum kembali ke menu

//===============================================   
// -- Academy(Shop) Scene Constants --
//===============================================
constexpr float ACADEMY_BASE_W = 1280.0f;
constexpr float ACADEMY_BASE_H = 720.0f;

// Path 4 frame Academy (idle + 3 hover state)
constexpr const char *ASSET_ACADEMY_FRAME_1 =
    "assets/sprites/ui/skill/Academy1.png";
constexpr const char *ASSET_ACADEMY_FRAME_2 =
    "assets/sprites/ui/skill/Academy2.png";
constexpr const char *ASSET_ACADEMY_FRAME_3 =
    "assets/sprites/ui/skill/Academy3.png";
constexpr const char *ASSET_ACADEMY_FRAME_4 =
    "assets/sprites/ui/skill/Academy4.png";

// Path icon skill
constexpr const char *ASSET_SKILL_ICON_01 =
    "assets/sprites/ui/skill/skill01.png";
constexpr const char *ASSET_SKILL_ICON_02 =
    "assets/sprites/ui/skill/skill02.png";
constexpr const char *ASSET_SKILL_ICON_03 =
    "assets/sprites/ui/skill/skill03.png";

// Hitbox 3 baris skill di panel kiri (rasio terhadap 1280×720)
// Format: { X_Ratio, Y_Ratio, Width_Ratio, Height_Ratio }
constexpr int ACADEMY_SKILL_COUNT = 3;
constexpr float ACADEMY_SKILL_HITBOXES[ACADEMY_SKILL_COUNT][4] = {
    {0.1688f, 0.3319f, 0.1953f, 0.0917f},  // skill 1 fire
    {0.1688f, 0.4278f, 0.1953f, 0.0917f},  // skill 2 ice
    {0.1688f, 0.5236f, 0.1953f, 0.0917f},  // skill 3 heal
};

// Hitbox tombol X (close) kanan atas
constexpr float ACADEMY_CLOSE_HITBOX[4] = {0.8203f, 0.1778f, 0.0523f, 0.0792f};

// Panel detail kanan (tempat render preview skill)
constexpr float ACADEMY_DETAIL_X = 0.63f;
constexpr float ACADEMY_DETAIL_Y = 0.27f;
constexpr float ACADEMY_DETAIL_W = 0.20f;
constexpr float ACADEMY_DETAIL_H = 0.45f;

// Ukuran icon skill di panel preview (pengali dari ukuran asli 60x54)
// Semakin besar = icon semakin besar. Default: 2.0f
constexpr float ACADEMY_ICON_SCALE = 1.3f;

// Ukuran font di panel preview (pengali dari tinggi layar)
// Semakin besar = teks semakin besar. Default: 0.03f
constexpr float ACADEMY_FONT_SCALE = 0.03f;

// 5 slot skill yang dimiliki player (kanan bawah)
constexpr int ACADEMY_OWNED_SLOT_COUNT = 5;
constexpr float ACADEMY_OWNED_SLOTS_X = 0.607f;   // X awal slot pertama
constexpr float ACADEMY_OWNED_SLOTS_Y = 0.624f;   // Y slot
constexpr float ACADEMY_OWNED_SLOT_W = 0.04605f;   // Lebar tiap slot
constexpr float ACADEMY_OWNED_SLOT_H = 0.08f;    // Tinggi tiap slot
constexpr float ACADEMY_OWNED_SLOT_GAP = 0.0035f; // Jarak antar slot

// Debug flag
constexpr bool ACADEMY_DEBUG_HITBOX = false;
