#pragma once
#include "constants.h"
#include "core/BaseScene.h"
#include "entities/Skill.h"
#include "raylib.h"
#include "types.h"
#include <string>
#include <vector>

// Hasil kalkulasi layout Academy (aspect-ratio aware)
struct AcademyLayout {
  float drawW;  // Lebar gambar setelah di-scale
  float drawH;  // Tinggi gambar setelah di-scale
  float imgX;   // Posisi X gambar di layar
  float imgY;   // Posisi Y gambar di layar
};

class AcademyScene : public BaseScene {
public:
  explicit AcademyScene(GameContext ctx);
  ~AcademyScene() override = default;

  void OnEnter() override;
  SceneType Update(float dt) override;
  void Draw() override;
  void OnExit() override;

private:
  // Hitung layout Academy berdasarkan ukuran layar saat ini
  AcademyLayout CalcLayout() const;

  GameContext context_;

  // 4 frame Academy (idle + 3 hover state)
  Texture2D academyFrames_[4] = {};

  // 3 icon skill
  Texture2D skillIcons_[ACADEMY_SKILL_COUNT] = {};

  // UI state
  int currentFrameIndex_ = 0; // 0=idle, 1-3=hover skill
  int hoveredSkill_ = -1;     // -1=none, 0-2=index skill

  // Data skill yang dijual
  std::vector<Skill> shopSkills_;

  // Feedback teks sementara (contoh: "Gold tidak cukup!")
  std::string feedbackText_;
  float feedbackTimer_ = 0.0f;
};
