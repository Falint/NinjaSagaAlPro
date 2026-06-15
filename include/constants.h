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
