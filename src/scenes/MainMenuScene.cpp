#include "scenes/MainMenuScene.h"
#include "constants.h"
#include "raylib.h"
#include <iostream>

// ─── Menu Button ID ──────────────────────────────────────────
enum class MenuButton { None, Play, Academy, Inventory, Shop, Exit };

// Jumlah tombol di menu
static constexpr int BUTTON_COUNT = 5;

// Y-ratio setiap tombol (sesuai posisi di PNG 1200x1200)
static constexpr float BUTTON_Y_RATIOS[BUTTON_COUNT] = {
    MENU_BTN_PLAY_Y_RATIO,
    MENU_BTN_ACADEMY_Y_RATIO,
    MENU_BTN_INVENTORY_Y_RATIO,
    MENU_BTN_SHOP_Y_RATIO,
    MENU_BTN_EXIT_Y_RATIO,
};



// ─── Helper: Hitung Rectangle Tombol ─────────────────────────
// Menghitung posisi tombol di layar berdasarkan posisi & skala gambar
static Rectangle GetButtonRect(float imgX, float imgY, float scale, int index) {
  float btnX = imgX + (MENU_BTN_X_RATIO * 1200.0f * scale);
  float btnY = imgY + (BUTTON_Y_RATIOS[index] * 1200.0f * scale);
  float btnW = MENU_BTN_W_RATIO * 1200.0f * scale;
  float btnH = MENU_BTN_H_RATIO * 1200.0f * scale;
  return {btnX, btnY, btnW, btnH};
}

// ─── Constructor ─────────────────────────────────────────────
MainMenuScene::MainMenuScene(GameContext ctx) : context_(ctx) {}

// ─── OnEnter ─────────────────────────────────────────────────
void MainMenuScene::OnEnter() {
  menuTexture_ = LoadTexture(ASSET_MAIN_MENU_BG);

  if (menuTexture_.id == 0) {
    std::cerr << "[ERROR] Gagal load texture: " << ASSET_MAIN_MENU_BG
              << std::endl;
  }
}

// ─── Update ──────────────────────────────────────────────────
SceneType MainMenuScene::Update(float dt) {
  (void)dt; // tidak dipakai saat ini

  int screenW = GetScreenWidth();
  int screenH = GetScreenHeight();

  // Hitung scaling: fit image height ke screen height
  float scale = static_cast<float>(screenH) / static_cast<float>(menuTexture_.height);
  float drawW = menuTexture_.width * scale;
  float drawH = menuTexture_.height * scale;

  // Posisi centered horizontal
  float imgX = (screenW - drawW) / 2.0f;
  float imgY = (screenH - drawH) / 2.0f;

  // Cek mouse position
  Vector2 mouse = GetMousePosition();

  for (int i = 0; i < BUTTON_COUNT; i++) {
    Rectangle btn = GetButtonRect(imgX, imgY, scale, i);

    if (CheckCollisionPointRec(mouse, btn) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

      switch (i) {
      case 0: // PLAY
        std::cout << "[MainMenu] PLAY clicked — scene belum tersedia."
                  << std::endl;
        break;
      case 1: // ACADEMY
        std::cout << "[MainMenu] ACADEMY clicked — scene belum tersedia."
                  << std::endl;
        break;
      case 2: // INVENTORY
        std::cout << "[MainMenu] INVENTORY clicked — scene belum tersedia."
                  << std::endl;
        break;
      case 3: // SHOP
        std::cout << "[MainMenu] SHOP clicked — scene belum tersedia."
                  << std::endl;
        break;
      case 4: // EXIT
        return SceneType::Exit;
      }
    }
  }

  return SceneType::None; // tetap di MainMenu
}

// ─── Draw ────────────────────────────────────────────────────
void MainMenuScene::Draw() {
  // ★ SET KE true UNTUK KALIBRASI HITBOX, false UNTUK PRODUCTION ★
  constexpr bool DEBUG_HITBOX = false;

  int screenW = GetScreenWidth();
  int screenH = GetScreenHeight();

  // Hitung scaling: fit image height ke screen height
  float scale =
      static_cast<float>(screenH) / static_cast<float>(menuTexture_.height);
  float drawW = menuTexture_.width * scale;
  float drawH = menuTexture_.height * scale;

  // Posisi centered horizontal
  float imgX = (screenW - drawW) / 2.0f;
  float imgY = (screenH - drawH) / 2.0f;

  // Source rectangle (seluruh gambar asli)
  Rectangle srcRect = {0, 0, static_cast<float>(menuTexture_.width),
                        static_cast<float>(menuTexture_.height)};
  // Destination rectangle (scaled & centered)
  Rectangle dstRect = {imgX, imgY, drawW, drawH};

  // Gambar menu PNG (base layer)
  DrawTexturePro(menuTexture_, srcRect, dstRect, {0, 0}, 0.0f, WHITE);

  // ── Hover effect — re-render tombol dari PNG itu sendiri ────
  Vector2 mouse = GetMousePosition();
  bool anyHover = false;
  float texW = static_cast<float>(menuTexture_.width);
  float texH = static_cast<float>(menuTexture_.height);

  // Warna debug per-tombol agar mudah dibedakan
  Color debugColors[BUTTON_COUNT] = {
      {255, 0, 0, 120},   // PLAY     - merah
      {0, 255, 0, 120},   // ACADEMY  - hijau
      {0, 0, 255, 120},   // INVENTORY- biru
      {255, 255, 0, 120}, // SHOP     - kuning
      {255, 0, 255, 120}, // EXIT     - ungu
  };

  for (int i = 0; i < BUTTON_COUNT; i++) {
    Rectangle btn = GetButtonRect(imgX, imgY, scale, i);

    // ── Debug: gambar outline hitbox ──
    if (DEBUG_HITBOX) {
      DrawRectangleRec(btn, debugColors[i]);
      DrawRectangleLinesEx(btn, 2.0f, WHITE);
    }

    if (CheckCollisionPointRec(mouse, btn)) {
      anyHover = true;

      // --- Source rect: area tombol di dalam texture asli ---
      float srcBtnX = MENU_BTN_X_RATIO * texW;
      float srcBtnY = BUTTON_Y_RATIOS[i] * texH;
      float srcBtnW = MENU_BTN_W_RATIO * texW;
      float srcBtnH = MENU_BTN_H_RATIO * texH;
      Rectangle btnSrc = {srcBtnX, srcBtnY, srcBtnW, srcBtnH};

      // --- Destination rect: tombol di layar, sedikit lebih besar ---
      float hoverScale = 1.08f;
      float newW = btn.width * hoverScale;
      float newH = btn.height * hoverScale;
      float newX = btn.x - (newW - btn.width) / 2.0f;
      float newY = btn.y - (newH - btn.height) / 2.0f;
      Rectangle btnDst = {newX, newY, newW, newH};

      Color hoverTint = {255, 240, 200, 255};
      DrawTexturePro(menuTexture_, btnSrc, btnDst, {0, 0}, 0.0f, hoverTint);

      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
  }

  if (!anyHover) {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
  }
}

// ─── OnExit ──────────────────────────────────────────────────
void MainMenuScene::OnExit() {
  if (menuTexture_.id != 0) {
    UnloadTexture(menuTexture_);
    menuTexture_ = {};
  }
}
