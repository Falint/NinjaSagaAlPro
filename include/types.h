#pragma once
#include "raylib.h"

enum class SceneType {
  None,
  Splash,
  MainMenu,
  MissionRank,
  Gameplay,
  Inventory,
  Academy,
  Exit
};

// Forward declaration — definisi lengkap di entities/Character.h
struct Character;

struct GameContext {
  Character *player = nullptr; // Pointer ke player global (dibuat di main.cpp)
};
