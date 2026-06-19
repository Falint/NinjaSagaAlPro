#include "scenes/InventoryScene.h"
#include "constants.h"
#include "raylib.h"
#include <algorithm>
#include <iostream>

InventoryScene::InventoryScene(GameContext ctx) : context_(ctx) {}

// ── Kalkulasi layout terpusat — tidak perlu copy-paste lagi ──
InvLayout InventoryScene::CalcLayout() const {
  int screenW = GetScreenWidth();
  int screenH = GetScreenHeight();

  float fitScale =
      static_cast<float>(std::min(screenW, screenH)) / INV_BASE_SIZE;
  float finalScale = fitScale * INV_SCALE_FACTOR;
  float drawSize = INV_BASE_SIZE * finalScale;

  float imgX = (static_cast<float>(screenW) - drawSize) / 2.0f;
  float imgY = (static_cast<float>(screenH) - drawSize) / 2.0f;

  float cellW = drawSize / static_cast<float>(INV_GRID_COLS);
  float cellH = drawSize / static_cast<float>(INV_GRID_ROWS);

  return {imgX, imgY, drawSize, cellW, cellH, finalScale};
}

// Kotak untuk GAMBAR tombol (visual)
static Rectangle GetBtnKembaliVisualRect(const InvLayout &layout) {
  float bx = layout.imgX + (layout.drawSize / 2.0f) +
             (INV_BTN_KEMBALI_VISUAL[0] * layout.drawSize);
  float by = layout.imgY + (INV_BTN_KEMBALI_VISUAL[1] * layout.drawSize);
  float bw = INV_BTN_KEMBALI_VISUAL[2] * layout.drawSize;
  float bh = INV_BTN_KEMBALI_VISUAL[3] * layout.drawSize;
  return {bx, by, bw, bh};
}

// Kotak untuk AREA KLIK tombol (hitbox)
static Rectangle GetBtnKembaliHitboxRect(const InvLayout &layout) {
  float bx = layout.imgX + (layout.drawSize / 2.0f) +
             (INV_BTN_KEMBALI_HITBOX[0] * layout.drawSize);
  float by = layout.imgY + (INV_BTN_KEMBALI_HITBOX[1] * layout.drawSize);
  float bw = INV_BTN_KEMBALI_HITBOX[2] * layout.drawSize;
  float bh = INV_BTN_KEMBALI_HITBOX[3] * layout.drawSize;
  return {bx, by, bw, bh};
}

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

  // Load texture karakter untuk animasi di samping inventory
  charTexture_ = LoadTexture(ASSET_CHARACTER_IDLE);
  if (charTexture_.id == 0) {
    std::cerr << "[ERROR] Gagal load texture: " << ASSET_CHARACTER_IDLE
              << std::endl;
  }

  // Load gambar button back
  btnKembaliTex_ = LoadTexture(ASSET_BTN_KEMBALI);
  if (btnKembaliTex_.id == 0) {
    std::cerr << "[ERROR] Gagal load texture: " << ASSET_BTN_KEMBALI
              << std::endl;
  }
}

SceneType InventoryScene::Update(float dt) {
  (void)dt;
  
  // ── Kembali ke Main Menu dengan ESC atau BACKSPACE 
  if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) {
    return SceneType::MainMenu;
  }
 
  // ── Hitung layout menggunakan helper terpusat ──
  InvLayout layout = CalcLayout();
 
  // ── Deteksi mouse ada di cell mana ──
  Vector2 mouse = GetMousePosition();
  hoveredSlot_ = -1;
 
  for (int i = 0; i < INV_SLOT_COUNT; i++) {
    int col = i % INV_GRID_COLS;
    int row = i / INV_GRID_COLS;
 
    float cellX = layout.imgX + static_cast<float>(col) * layout.cellW;
    float cellY = layout.imgY + static_cast<float>(row) * layout.cellH;
 
    float hx = cellX + (INV_SLOT_HITBOXES[i][0] * layout.cellW);
    float hy = cellY + (INV_SLOT_HITBOXES[i][1] * layout.cellH);
    float hw = INV_SLOT_HITBOXES[i][2] * layout.cellW;
    float hh = INV_SLOT_HITBOXES[i][3] * layout.cellH;
 
    Rectangle hitboxRect = {hx, hy, hw, hh};
 
    if (CheckCollisionPointRec(mouse, hitboxRect)) {
      hoveredSlot_ = i;
      break; 
    }
  }
   //Tombol Kembali klik 
    if (btnKembaliTex_.id != 0) {
      Rectangle btnR = GetBtnKembaliHitboxRect(layout);
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        CheckCollisionPointRec(mouse, btnR)) {
        return SceneType::MainMenu;
      }
    }

  // ── Update Animasi Karakter ──
  // Kecepatan animasi (berapa frame per detik)
  int framesSpeed = 8;

  // Tambah waktu penghitung setiap frame (berdasarkan 60 FPS dari Raylib)
  charFramesCounter_++;

  // Jika penghitung sudah melampaui batas kecepatan untuk frame selanjutnya
  if (charFramesCounter_ >= (60 / framesSpeed)) {
    charFramesCounter_ = 0; // Reset penghitung
    charCurrentFrame_++;    // Pindah ke frame animasi berikutnya

    // Jika animasi mencapai akhir, kembali ke frame pertama (loop)
    if (charCurrentFrame_ >= charNumFrames_) {
      charCurrentFrame_ = 0;
    }
  }

  return SceneType::None;
}

void InventoryScene::Draw() {
  ClearBackground(BLACK);

  // ── Hitung layout menggunakan helper terpusat ──
  InvLayout layout = CalcLayout();

  // ── 1. Draw Inv.png sebagai background (centered + scaled) ──
  Rectangle srcRect = {0, 0, static_cast<float>(invTexture_.width),
                       static_cast<float>(invTexture_.height)};
  Rectangle dstRect = {layout.imgX, layout.imgY, layout.drawSize,
                       layout.drawSize};

  DrawTexturePro(invTexture_, srcRect, dstRect, {0, 0}, 0.0f, WHITE);

  // ── 2. Draw Inv_hitbox.png di atas slot yang di-hover ──
  if (hoveredSlot_ >= 0 && hoveredSlot_ < INV_SLOT_COUNT) {
    int col = hoveredSlot_ % INV_GRID_COLS;
    int row = hoveredSlot_ / INV_GRID_COLS;

    float cellX = layout.imgX + static_cast<float>(col) * layout.cellW;
    float cellY = layout.imgY + static_cast<float>(row) * layout.cellH;

    // Render Inv_hitbox.png sesuai nilai hitbox di constants.h
    Rectangle hitboxSrc = {0, 0, static_cast<float>(hitboxTexture_.width),
                           static_cast<float>(hitboxTexture_.height)};

    float hx = cellX + (INV_SLOT_HITBOXES[hoveredSlot_][0] * layout.cellW);
    float hy = cellY + (INV_SLOT_HITBOXES[hoveredSlot_][1] * layout.cellH);
    float hw = INV_SLOT_HITBOXES[hoveredSlot_][2] * layout.cellW;
    float hh = INV_SLOT_HITBOXES[hoveredSlot_][3] * layout.cellH;

    Rectangle hitboxDst = {hx, hy, hw, hh};

    DrawTexturePro(hitboxTexture_, hitboxSrc, hitboxDst, {0, 0}, 0.0f, WHITE);

    // Ubah cursor jadi pointing hand
    SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
  } else {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
  }

  // ── 3. Draw Animasi Karakter di Samping Kiri Inventory ──
  if (charTexture_.id != 0) {
    // Hitung ukuran lebar untuk satu frame (total lebar dibagi jumlah frame)
    float frameWidth = static_cast<float>(charTexture_.width) /
                       static_cast<float>(charNumFrames_);
    float frameHeight = static_cast<float>(charTexture_.height);

    // Bingkai/Jendela untuk memotong gambar ke satu frame saja
    Rectangle charSrcRect = {static_cast<float>(charCurrentFrame_) *
                                 frameWidth, // Posisi X bergeser tiap frame
                             0.0f,           // Posisi Y tetap (atas)
                             frameWidth, frameHeight};

    // Tentukan besar skala karakter menggunakan konstant INV_CHAR_SCALE
    float charScale = layout.finalScale * INV_CHAR_SCALE;
    float charDrawWidth = frameWidth * charScale;
    float charDrawHeight = frameHeight * charScale;

    // Hitung posisi Karakter: tepat di tengah-tengah inventory ditambah Offset
    float charX = layout.imgX + (layout.drawSize / 2.0f) -
                  (charDrawWidth / 2.0f) + INV_CHAR_OFFSET_X;

    // Vertikal: Posisikan tepat di atas inventory dikurangi Offset Y
    float charY = layout.imgY - charDrawHeight - INV_CHAR_OFFSET_Y;

    Rectangle charDstRect = {charX, charY, charDrawWidth, charDrawHeight};

    // Gambar karakter animasi ke layar
    DrawTexturePro(charTexture_, charSrcRect, charDstRect, {0, 0}, 0.0f, WHITE);

    // +++ DEBUG HITBOX KARAKTER +++
    if (INV_CHAR_DEBUG_HITBOX) {
      DrawRectangleLinesEx(charDstRect, 2.0f, RED);
      DrawText("CHAR HITBOX", static_cast<int>(charX),
               static_cast<int>(charY - 20), 20, RED);
    }
  }

  // ── Debug: tampilkan kotak hitbox berwarna untuk kalibrasi ──
  if (INV_DEBUG_HITBOX) {
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

      float cellX = layout.imgX + static_cast<float>(col) * layout.cellW;
      float cellY = layout.imgY + static_cast<float>(row) * layout.cellH;

      float hx = cellX + (INV_SLOT_HITBOXES[i][0] * layout.cellW);
      float hy = cellY + (INV_SLOT_HITBOXES[i][1] * layout.cellH);
      float hw = INV_SLOT_HITBOXES[i][2] * layout.cellW;
      float hh = INV_SLOT_HITBOXES[i][3] * layout.cellH;

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
  
  // Draw tombol kembali
  // Draw tombol kembali
  if (btnKembaliTex_.id != 0) {
      Rectangle visualR = GetBtnKembaliVisualRect(layout);   // untuk gambar
      Rectangle hitboxR = GetBtnKembaliHitboxRect(layout);   // untuk klik
      bool hov = CheckCollisionPointRec(GetMousePosition(), hitboxR);
      if (hov) {
          SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
      }

      Rectangle src = { 0, 0, (float)btnKembaliTex_.width, (float)btnKembaliTex_.height };
      DrawTexturePro(btnKembaliTex_, src, visualR, {0,0}, 0.0f,
          hov ? Color{255,255,255,200} : WHITE);
      if (INV_BTN_KEMBALI_DEBUG_HITBOX) {
        DrawRectangleLinesEx(hitboxR, 2.0f, RED);   // garis merah = area klik
        DrawRectangleLinesEx(visualR, 2.0f, GREEN); // garis hijau = area gambar
      }
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
  if (btnKembaliTex_.id != 0) {
    UnloadTexture(btnKembaliTex_);
    btnKembaliTex_ = {};
  }
  // Unload texture karakter saat keluar dari Scene Inventory
  if (charTexture_.id != 0) {
    UnloadTexture(charTexture_);
    charTexture_ = {};
  }
  hoveredSlot_ = -1;
  charCurrentFrame_ = 0;
  charFramesCounter_ = 0;
}
