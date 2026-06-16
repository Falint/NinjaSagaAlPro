#include "scenes/InventoryScene.h"
#include "constants.h"
#include "raylib.h"
#include <iostream>

InventoryScene::InventoryScene(GameContext ctx) : context_(ctx) {}

void InventoryScene::OnEnter() {
  // Load background inventory (grid 3x3)
  invTexture_ = LoadTexture(ASSET_INVENTORY_FRAME);
  if (invTexture_.id == 0) {
    std::cerr << "[ERROR] Gagal load texture: " << ASSET_INVENTORY_FRAME
              << std::endl;
  }

  // Load overlay highlight yang akan berpindah ke slot yang di-hover
  hitboxTexture_ = LoadTexture(ASSET_INVENTORY_HITBOX);
  if (hitboxTexture_.id == 0) {
    std::cerr << "[ERROR] Gagal load texture: " << ASSET_INVENTORY_HITBOX
              << std::endl;
  }
}

SceneType InventoryScene::Update(float dt) {
  (void)dt;

  // Kembali ke Main Menu dengan ESC atau BACKSPACE
  if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) {
    return SceneType::MainMenu;
  }

  // ── Hitung posisi & ukuran inventory yang sudah di-scale ──
  int screenW = GetScreenWidth();
  int screenH = GetScreenHeight();

  float fitScale =
      static_cast<float>(screenH < screenW ? screenH : screenW) / INV_BASE_SIZE;
  float finalScale = fitScale * INV_SCALE_FACTOR;
  float drawSize = INV_BASE_SIZE * finalScale;

  // Posisi centered di layar
  float imgX = (static_cast<float>(screenW) - drawSize) / 2.0f;
  float imgY = (static_cast<float>(screenH) - drawSize) / 2.0f;

  // Ukuran setiap cell dalam grid 3x3
  float cellW = drawSize / static_cast<float>(INV_GRID_COLS);
  float cellH = drawSize / static_cast<float>(INV_GRID_ROWS);

  // ── Deteksi mouse ada di cell mana ──
  Vector2 mouse = GetMousePosition();
  hoveredSlot_ = -1;

  for (int i = 0; i < INV_SLOT_COUNT; i++) {
    int col = i % INV_GRID_COLS;
    int row = i / INV_GRID_COLS;

    float cellX = imgX + static_cast<float>(col) * cellW;
    float cellY = imgY + static_cast<float>(row) * cellH;

    Rectangle cellRect = {cellX, cellY, cellW, cellH};

    if (CheckCollisionPointRec(mouse, cellRect)) {
      hoveredSlot_ = i;
      break;
    }
  }

  return SceneType::None;
}

void InventoryScene::Draw() {
  // Flag debug: set true untuk melihat kotak hitbox berwarna
  constexpr bool DEBUG_HITBOX = true;

  int screenW = GetScreenWidth();
  int screenH = GetScreenHeight();

  ClearBackground(BLACK);

  // ── Hitung posisi & ukuran (sama seperti di Update) ──
  float fitScale =
      static_cast<float>(screenH < screenW ? screenH : screenW) / INV_BASE_SIZE;
  float finalScale = fitScale * INV_SCALE_FACTOR;
  float drawSize = INV_BASE_SIZE * finalScale;

  float imgX = (static_cast<float>(screenW) - drawSize) / 2.0f;
  float imgY = (static_cast<float>(screenH) - drawSize) / 2.0f;

  float cellW = drawSize / static_cast<float>(INV_GRID_COLS);
  float cellH = drawSize / static_cast<float>(INV_GRID_ROWS);

  // ── 1. Draw Inv.png sebagai background (centered + scaled) ──
  Rectangle srcRect = {0, 0, static_cast<float>(invTexture_.width),
                       static_cast<float>(invTexture_.height)};
  Rectangle dstRect = {imgX, imgY, drawSize, drawSize};

  DrawTexturePro(invTexture_, srcRect, dstRect, {0, 0}, 0.0f, WHITE);

  // ── 2. Draw Inv_hitbox.png di atas slot yang di-hover ──
  if (hoveredSlot_ >= 0 && hoveredSlot_ < INV_SLOT_COUNT) {
    int col = hoveredSlot_ % INV_GRID_COLS;
    int row = hoveredSlot_ / INV_GRID_COLS;

    float cellX = imgX + static_cast<float>(col) * cellW;
    float cellY = imgY + static_cast<float>(row) * cellH;

    // Render Inv_hitbox.png di-scale ke ukuran cell, tepat di posisi slot
    Rectangle hitboxSrc = {0, 0, static_cast<float>(hitboxTexture_.width),
                           static_cast<float>(hitboxTexture_.height)};
    Rectangle hitboxDst = {cellX, cellY, cellW, cellH};

    DrawTexturePro(hitboxTexture_, hitboxSrc, hitboxDst, {0, 0}, 0.0f, WHITE);

    // Ubah cursor jadi pointing hand
    SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
  } else {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
  }

  // ── Debug: tampilkan kotak hitbox berwarna untuk kalibrasi ──
  if (DEBUG_HITBOX) {
    Color slotColors[INV_SLOT_COUNT] = {
        {255, 0, 0, 100},     // Slot 0 - Merah
        {0, 255, 0, 100},     // Slot 1 - Hijau
        {0, 0, 255, 100},     // Slot 2 - Biru
        {255, 255, 0, 100},   // Slot 3 - Kuning
        {255, 0, 255, 100},   // Slot 4 - Magenta
        {0, 255, 255, 100},   // Slot 5 - Cyan
        {255, 128, 0, 100},   // Slot 6 - Oranye
        {128, 0, 255, 100},   // Slot 7 - Ungu
        {255, 128, 128, 100}, // Slot 8 - Pink
    };

    for (int i = 0; i < INV_SLOT_COUNT; i++) {
      int col = i % INV_GRID_COLS;
      int row = i / INV_GRID_COLS;

      float cellX = imgX + static_cast<float>(col) * cellW;
      float cellY = imgY + static_cast<float>(row) * cellH;

      float hx = cellX + (INV_SLOT_HITBOXES[i][0] * cellW);
      float hy = cellY + (INV_SLOT_HITBOXES[i][1] * cellH);
      float hw = INV_SLOT_HITBOXES[i][2] * cellW;
      float hh = INV_SLOT_HITBOXES[i][3] * cellH;

      Rectangle rect = {hx, hy, hw, hh};
      DrawRectangleRec(rect, slotColors[i]);
      DrawRectangleLinesEx(rect, 1.0f, WHITE);
    }
  }

  // ── Info text ──

  if (hoveredSlot_ >= 0) {
    const char *slotText = TextFormat("Hover: Slot %d", hoveredSlot_);
    DrawText(slotText, 20, 50, 20, GREEN);
  }
}

void InventoryScene::OnExit() {
  if (invTexture_.id != 0) {
    UnloadTexture(invTexture_);
    invTexture_ = {};
  }
  if (hitboxTexture_.id != 0) {
    UnloadTexture(hitboxTexture_);
    hitboxTexture_ = {};
  }
  hoveredSlot_ = -1;
}
