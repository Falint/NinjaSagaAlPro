#pragma once
#include "raylib.h"
#include <string>
#include <vector>

using namespace std;

enum class SceneType {
  None,
  Splash,
  MainMenu,
  Gameplay,
  Inventory,
  Exit
};

struct GameContext {
  // Add any global game state or configurations here later.
};
