#pragma once

// -- Splash Scene Constants --
constexpr float SPLASH_FADE_IN_DURATION = 1.0f;
constexpr float SPLASH_HOLD_DURATION = 2.0f;
constexpr float SPLASH_FADE_OUT_DURATION = 1.0f;
constexpr float SPLASH_TOTAL_DURATION =
    SPLASH_FADE_IN_DURATION + SPLASH_HOLD_DURATION + SPLASH_FADE_OUT_DURATION;
constexpr int SPLASH_TITLE_FONT_SIZE = 80;

// -- Main Menu Scene Constants --
// -- Main Menu Scene Constants --
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

// -- Inventory Scene Constants --
constexpr const char *ASSET_INVENTORY_FRAME = "assets/sprites/ui/Inv.png";
constexpr const char *ASSET_INVENTORY_HITBOX =
    "assets/sprites/ui/Inv_hitbox.png";
constexpr const char *ASSET_CHARACTER_IDLE = "assets/sprites/ui/idle_down.png";

// Offset dan Scale untuk karakter animasi di UI Inventory
constexpr float INV_CHAR_SCALE =
    1.5f; // Pengali ukuran karakter (semakin besar makin raksasa)
constexpr float INV_CHAR_OFFSET_X =
    0.0f; // Jarak geser horizontal (0 = tepat di tengah)
constexpr float INV_CHAR_OFFSET_Y =
    -100.0f; // Jarak karakter dari atas UI inventory

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

constexpr bool DEBUG_HITBOX = false;
// Per-slot hitbox (rasio relatif terhadap cell size masing-masing)
// Dari analisis Inv_hitbox.png (64x64): inner area pixel (18,18)→(45,47)
// Format: { X_Ratio, Y_Ratio, Width_Ratio, Height_Ratio }
// Setiap slot bisa diubah sendiri tanpa mempengaruhi slot lain
constexpr float INV_SLOT_HITBOXES[INV_SLOT_COUNT][4] = {
    {0.48f, 0.28f, 0.74f, 0.47f}, // Slot 0 (kiri atas)
    {0.28f, 0.28f, 0.74f, 0.47f}, // Slot 1 (tengah atas)
    {0.28f, 0.28f, 0.74f, 0.47f}, // Slot 2 (kanan atas)
    {0.28f, 0.28f, 0.74f, 0.47f}, // Slot 3 (kiri tengah)
    {0.28f, 0.28f, 0.74f, 0.47f}, // Slot 4 (tengah)
    {0.28f, 0.28f, 0.74f, 0.47f}, // Slot 5 (kanan tengah)
    {0.28f, 0.28f, 0.74f, 0.47f}, // Slot 6 (kiri bawah)
    {0.28f, 0.28f, 0.74f, 0.47f}, // Slot 7 (tengah bawah)
    {0.28f, 0.28f, 0.74f, 0.47f}, // Slot 8 (kanan bawah)
};

// -- Battle Scene Constants --

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
