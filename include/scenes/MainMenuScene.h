
#pragma once
#include "core/BaseScene.h"
#include "raylib.h"
#include "types.h"

// Struct untuk menyimpan hasil kalkulasi layout menu
struct MenuLayout {
  float drawW;  // Lebar gambar setelah di-scale
  float drawH;  // Tinggi gambar setelah di-scale
  float imgX;   // Posisi X gambar di layar
  float imgY;   // Posisi Y gambar di layar
};

class MainMenuScene : public BaseScene {
public:
  explicit MainMenuScene(GameContext ctx);
  ~MainMenuScene() override = default;

  void OnEnter() override;
  SceneType Update(float dt) override;
  void Draw() override;
  void OnExit() override;

private:
  // Hitung layout menu berdasarkan ukuran layar saat ini
  MenuLayout CalcLayout() const;

  GameContext context_;
  Texture2D menuFrames_[6] = {};
  int currentFrameIndex_ = 0;
};
