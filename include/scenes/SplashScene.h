
#pragma once
#include "core/BaseScene.h"
#include "types.h"

class SplashScene : public BaseScene {
public:
  explicit SplashScene(GameContext ctx);
  ~SplashScene() override = default;

  void OnEnter() override;
  SceneType Update(float dt) override;
  void Draw() override;
  void OnExit() override;

private:
  GameContext context_;
  float timer_;
  float alpha_;
};
