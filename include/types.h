#pragma once
#include "raylib.h"

enum class SceneType {
  None,
  Splash,
  MainMenu,
  Gameplay,
  Inventory,
  Shop,
  Exit
};

struct GameContext {
  // Add any global game state or configurations here later.
};
