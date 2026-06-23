#include "scenes/SplashScene.h"
#include "constants.h"
#include "raylib.h"
#include <cmath>

// ─── Constructor ─────────────────────────────────────────────
SplashScene::SplashScene(GameContext ctx)
    : context_(ctx), timer_(0.0f), alpha_(0.0f) {}

// ─── OnEnter ─────────────────────────────────────────────────
void SplashScene::OnEnter() {
  timer_ = 0.0f;
  alpha_ = 0.0f;
}

// ─── Update ──────────────────────────────────────────────────
SceneType SplashScene::Update(float dt) {
  timer_ += dt;

  // Skip splash jika user menekan tombol apa saja atau klik mouse
  if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) ||
      IsKeyPressed(KEY_ESCAPE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    return SceneType::MainMenu;
  }

  // Phase 1: Fade In
  if (timer_ < SPLASH_FADE_IN_DURATION) {
    alpha_ = timer_ / SPLASH_FADE_IN_DURATION;
  }
  // Phase 2: Hold indefinitely (menunggu input user)
  else {
    alpha_ = 1.0f;
  }

  return SceneType::None; // tetap di splash
}

// ─── Draw ────────────────────────────────────────────────────
void SplashScene::Draw() {
  int screenW = GetScreenWidth();
  int screenH = GetScreenHeight();

  // Clamp alpha ke 0-255
  unsigned char a = static_cast<unsigned char>(alpha_ * 255.0f);

  // Warna teks dengan alpha
  Color titleColor = {0, 228, 255, a}; // Biru Neon
  Color titleShadow = {180, 180, 180, a}; // Putih agak abu-abu
  Color subColor = {200, 200, 200, a};  // Abu-abu terang

  // Title "NINJA SAGA"
  const char *title = "NINJA SAGA";
  int titleSize = SPLASH_TITLE_FONT_SIZE;
  int titleWidth = MeasureText(title, titleSize);
  int titleX = (screenW - titleWidth) / 2;
  int titleY = (screenH / 2) - titleSize;
  
  // Gambar bayangan untuk NINJA SAGA
  DrawText(title, titleX + 4, titleY + 4, titleSize, titleShadow);
  // Gambar teks utama NINJA SAGA
  DrawText(title, titleX, titleY, titleSize, titleColor);

  // Subtitle "AlPro"
  const char *alpro = "AlPro";
  int alproSize = static_cast<int>(titleSize * 0.7f); // Sedikit lebih besar dari sebelumnya
  int alproWidth = MeasureText(alpro, alproSize);
  int alproX = (screenW - alproWidth) / 2;
  int alproY = titleY + titleSize + 10;
  
  Color alproColor = {220, 220, 220, a}; // Abu-abu terang
  
  // Gambar teks utamanya saja untuk AlPro (tanpa shadow tebal seperti sebelumnya)
  DrawText(alpro, alproX, alproY, alproSize, alproColor);

  // Subtitle instruksi
  const char *subtitle = "Press any key to continue...";
  int subSize = 20;
  int subWidth = MeasureText(subtitle, subSize);
  int subX = (screenW - subWidth) / 2;
  int subY = alproY + alproSize + 40;

  // Subtitle hanya muncul saat hold phase (sudah fully visible)
  if (timer_ >= SPLASH_FADE_IN_DURATION) {
    // Efek kedip halus untuk subtitle
    float blink = (sinf(timer_ * 3.0f) + 1.0f) / 2.0f;
    unsigned char subAlpha =
        static_cast<unsigned char>(blink * alpha_ * 255.0f);
    subColor.a = subAlpha;
    DrawText(subtitle, subX, subY, subSize, subColor);
  }
}

// ─── OnExit ──────────────────────────────────────────────────
void SplashScene::OnExit() {
  // Tidak ada resource yang perlu di-cleanup
}
