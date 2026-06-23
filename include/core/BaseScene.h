
#pragma once
#include "types.h"

class BaseScene {
public:
  virtual ~BaseScene() = default;
  virtual void OnEnter() {}
  virtual SceneType Update(float dt) = 0;
  virtual void Draw() = 0;
  virtual void OnExit() {}
};
