#include "scenes/MainMenuScene.h"
#include "constants.h"
#include "raylib.h"
#include "types.h"
#include <iostream>

// ─── Menu Button ID ──────────────────────────────────────────
enum class MenuButton { None, Play, Shop, Inventory, Status, Exit };

// ─── Constructor ─────────────────────────────────────────────
MainMenuScene::MainMenuScene(GameContext ctx) : context_(ctx) {}

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

  int screenW = GetScreenWidth();
  int screenH = GetScreenHeight();
  Vector2 mouse = GetMousePosition();

  // Hitung scaling dari 200x200 ke ukuran layar agar aspect ratio tetap terjaga
  // (fit ke height)
  float scale = static_cast<float>(screenH) / MENU_BASE_SIZE;
  float drawW = MENU_BASE_SIZE * scale;
  float drawH = MENU_BASE_SIZE * scale;
  float imgX = (screenW - drawW) / 2.0f;
  float imgY = (screenH - drawH) / 2.0f;

  currentFrameIndex_ = 0; // Default frame (Idle)

  for (int i = 0; i < BUTTON_COUNT; i++) {
    // Hitung hitbox riil di layar berdasarkan rasio 0.0 - 1.0 dari gambar
    // 200x200
    float bx = imgX + (MENU_HITBOXES[i][0] * drawW);
    float by = imgY + (MENU_HITBOXES[i][1] * drawH);
    float bw = MENU_HITBOXES[i][2] * drawW;
    float bh = MENU_HITBOXES[i][3] * drawH;
    Rectangle btn = {bx, by, bw, bh};

    if (CheckCollisionPointRec(mouse, btn)) {
      // 1 = idle, 2 = play, 3 = shop, 4 = inventory, 5 = status, 6 = exit
      // Karena index array mulai dari 0:
      // i=0 (Play) -> frame 2 -> index 1
      // i=1 (Shop) -> frame 3 -> index 2
      // i=2 (Inventory) -> frame 4 -> index 3
      // i=3 (Status) -> frame 5 -> index 4
      // i=4 (Exit) -> frame 6 -> index 5
      currentFrameIndex_ = i + 1;

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        switch (i) {
        case 0: // PLAY
          std::cout << "[MainMenu] PLAY clicked" << std::endl;
          break;
        case 1: // SHOP
          std::cout << "[MainMenu] SHOP clicked" << std::endl;
          break;
        case 2: // INVENTORY
          std::cout << "[MainMenu] INVENTORY clicked -> Go To Inventory"
                    << std::endl;
          return SceneType::Inventory;
        case 3: // STATUS
          std::cout << "[MainMenu] STATUS clicked" << std::endl;
          break;
        case 4: // EXIT
          return SceneType::Exit;
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
  constexpr bool DEBUG_HITBOX = false; // Matikan warna hitbox

  int screenW = GetScreenWidth();
  int screenH = GetScreenHeight();

  // Draw HANYA SATU gambar fullscreen
  Texture2D texToDraw = menuFrames_[currentFrameIndex_];

  float scale = static_cast<float>(screenH) / MENU_BASE_SIZE;
  float drawW = MENU_BASE_SIZE * scale;
  float drawH = MENU_BASE_SIZE * scale;
  float imgX = (screenW - drawW) / 2.0f;
  float imgY = (screenH - drawH) / 2.0f;

  Rectangle srcRect = {0, 0, static_cast<float>(texToDraw.width),
                       static_cast<float>(texToDraw.height)};
  Rectangle dstRect = {imgX, imgY, drawW, drawH};

  DrawTexturePro(texToDraw, srcRect, dstRect, {0, 0}, 0.0f, WHITE);

  if (currentFrameIndex_ != 0) {
    SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
  } else {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
  }

  // ── Debug Hitbox Berwarna ──
  if (DEBUG_HITBOX) {
    Color debugColors[BUTTON_COUNT] = {
        {255, 0, 0, 150},   // PLAY - Merah
        {0, 255, 0, 150},   // SHOP - Hijau
        {0, 0, 255, 150},   // INVENTORY - Biru
        {255, 255, 0, 150}, // STATUS - Kuning
        {255, 0, 255, 150}, // EXIT - Ungu
    };

    for (int i = 0; i < BUTTON_COUNT; i++) {
      float bx = imgX + (MENU_HITBOXES[i][0] * drawW);
      float by = imgY + (MENU_HITBOXES[i][1] * drawH);
      float bw = MENU_HITBOXES[i][2] * drawW;
      float bh = MENU_HITBOXES[i][3] * drawH;

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
