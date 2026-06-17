#include "scenes/MainMenuScene.h"
#include "constants.h"
#include "raylib.h"
#include "types.h"
#include <iostream>

// ─── Menu Button ID ──────────────────────────────────────────
enum class MenuButton { None, Play, Shop, Inventory, Status, Exit };

// ─── Constructor ─────────────────────────────────────────────
MainMenuScene::MainMenuScene(GameContext ctx) : context_(ctx) {}

// ── Kalkulasi layout terpusat — tidak perlu copy-paste lagi ──
MenuLayout MainMenuScene::CalcLayout() const {
  int screenW = GetScreenWidth();
  int screenH = GetScreenHeight();

  float scale = static_cast<float>(screenH) / MENU_BASE_SIZE;
  float drawW = MENU_BASE_SIZE * scale;
  float drawH = MENU_BASE_SIZE * scale;
  float imgX = (static_cast<float>(screenW) - drawW) / 2.0f;
  float imgY = (static_cast<float>(screenH) - drawH) / 2.0f;

  return {drawW, drawH, imgX, imgY};
}

// ─── OnEnter ─────────────────────────────────────────────────
void MainMenuScene::OnEnter() {
  menuFrames_[0] = LoadTexture(ASSET_MENU_FRAME_1);
  menuFrames_[1] = LoadTexture(ASSET_MENU_FRAME_2);
  menuFrames_[2] = LoadTexture(ASSET_MENU_FRAME_3);
  menuFrames_[3] = LoadTexture(ASSET_MENU_FRAME_4);
  menuFrames_[4] = LoadTexture(ASSET_MENU_FRAME_5);
  menuFrames_[5] = LoadTexture(ASSET_MENU_FRAME_6);

  for (int i = 0; i < 6; i++) {
    if (menuFrames_[i].id == 0) {
      std::cerr << "[ERROR] Gagal load texture frame index: " << i << std::endl;
    }
  }
}

// ─── Update ──────────────────────────────────────────────────
SceneType MainMenuScene::Update(float dt) {
  (void)dt;

  if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) {
    return SceneType::Exit;
  }

  Vector2 mouse = GetMousePosition();

  // ── Hitung layout menggunakan helper terpusat ──
  MenuLayout layout = CalcLayout();

  currentFrameIndex_ = 0; // Default frame (Idle)

  for (int i = 0; i < BUTTON_COUNT; i++) {
    // Hitung hitbox riil di layar berdasarkan rasio 0.0 - 1.0 dari gambar
    float bx = layout.imgX + (MENU_HITBOXES[i][0] * layout.drawW);
    float by = layout.imgY + (MENU_HITBOXES[i][1] * layout.drawH);
    float bw = MENU_HITBOXES[i][2] * layout.drawW;
    float bh = MENU_HITBOXES[i][3] * layout.drawH;
    Rectangle btn = {bx, by, bw, bh};

    if (CheckCollisionPointRec(mouse, btn)) {
      // i=0 (Play) -> frame index 1, i=1 (Shop) -> frame index 2, dst.
      currentFrameIndex_ = i + 1;

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // Gunakan enum MenuButton agar kode lebih deskriptif
        MenuButton clicked = static_cast<MenuButton>(i + 1);
        switch (clicked) {
        case MenuButton::Play:
          std::cout << "[MainMenu] PLAY clicked -> Go to Battle" << std::endl;
          return SceneType::Gameplay;
        case MenuButton::Shop:
          std::cout << "[MainMenu] SHOP clicked -> Go To Shop" << std::endl;
          return SceneType::Shop;
        case MenuButton::Inventory:
          std::cout << "[MainMenu] INVENTORY clicked -> Go To Inventory"
                    << std::endl;
          return SceneType::Inventory;
        case MenuButton::Status:
          std::cout << "[MainMenu] STATUS clicked" << std::endl;
          break;
        case MenuButton::Exit:
          return SceneType::Exit;
        default:
          break;
        }
      }
    }
  }

  // Clamping agar index tidak out of bounds
  if (currentFrameIndex_ > 5)
    currentFrameIndex_ = 5;

  return SceneType::None;
}

// ─── Draw ────────────────────────────────────────────────────
void MainMenuScene::Draw() {
  // Draw HANYA SATU gambar fullscreen
  Texture2D texToDraw = menuFrames_[currentFrameIndex_];

  // ── Hitung layout menggunakan helper terpusat ──
  MenuLayout layout = CalcLayout();

  Rectangle srcRect = {0, 0, static_cast<float>(texToDraw.width),
                       static_cast<float>(texToDraw.height)};
  Rectangle dstRect = {layout.imgX, layout.imgY, layout.drawW, layout.drawH};

  DrawTexturePro(texToDraw, srcRect, dstRect, {0, 0}, 0.0f, WHITE);

  if (currentFrameIndex_ != 0) {
    SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
  } else {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
  }

  // ── Debug Hitbox Berwarna ──
  if (MENU_DEBUG_HITBOX) {
    Color debugColors[BUTTON_COUNT] = {
        {255, 0, 0, 150},   // PLAY - Merah
        {0, 255, 0, 150},   // SHOP - Hijau
        {0, 0, 255, 150},   // INVENTORY - Biru
        {255, 255, 0, 150}, // STATUS - Kuning
        {255, 0, 255, 150}, // EXIT - Ungu
    };

    for (int i = 0; i < BUTTON_COUNT; i++) {
      float bx = layout.imgX + (MENU_HITBOXES[i][0] * layout.drawW);
      float by = layout.imgY + (MENU_HITBOXES[i][1] * layout.drawH);
      float bw = MENU_HITBOXES[i][2] * layout.drawW;
      float bh = MENU_HITBOXES[i][3] * layout.drawH;

      Rectangle rect = {bx, by, bw, bh};
      DrawRectangleRec(rect, debugColors[i]);  // Fill transparan
      DrawRectangleLinesEx(rect, 2.0f, WHITE); // Border putih
    }
  }
}

// ─── OnExit ──────────────────────────────────────────────────
void MainMenuScene::OnExit() {
  for (int i = 0; i < 6; i++) {
    if (menuFrames_[i].id != 0) {
      UnloadTexture(menuFrames_[i]);
      menuFrames_[i] = {};
    }
  }
}
