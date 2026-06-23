#pragma once
#include "core/BaseScene.h"
#include "raylib.h"
#include "types.h"
#include <string>

struct MissionLayout {
  float drawW;  // Lebar gambar setelah di-scale
  float drawH;  // Tinggi gambar setelah di-scale
  float imgX;   // Posisi X gambar di layar
  float imgY;   // Posisi Y gambar di layar
};

class MissionRankScene : public BaseScene {
public:
  explicit MissionRankScene(GameContext ctx);
  ~MissionRankScene() override = default;

  void OnEnter() override;
  SceneType Update(float dt) override;
  void Draw() override;
  void OnExit() override;

private:
  MissionLayout CalcLayout() const;
  Texture2D menuBackgroundTex_ = {};
  GameContext context_;
  Texture2D missionFrames_[6] = {};
  int currentFrameIndex_ = 0;
  bool showLockedWarning_ = false;
  float warningTimer_ = 0.0f;
  std::string lockedRankName_ = "";
};
