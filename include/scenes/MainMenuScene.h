
#pragma once
#include "core/BaseScene.h"
#include "raylib.h"
#include "types.h"

class MainMenuScene : public BaseScene {
public:
  explicit MainMenuScene(GameContext ctx);
  ~MainMenuScene() override = default;

  void OnEnter() override;
  SceneType Update(float dt) override;
  void Draw() override;
  void OnExit() override;

private:
  GameContext context_;
  Texture2D menuFrames_[6] = {};
  int currentFrameIndex_ = 0;
};
