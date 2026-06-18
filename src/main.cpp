#include "core/SceneManager.h"
#include "entities/Character.h"
#include "raylib.h"
#include "types.h"
#include <iostream>

int main() {
  std::cout << "Starting Ninja Saga..." << std::endl;
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1200, 720, "Ninja Saga");
  SetExitKey(KEY_NULL);
  SetTargetFPS(60);
  InitAudioDevice();

  // Buat player global — data ini bertahan selama game berjalan
  Character player;
  player.name = "Ninja";
  player.hp = 10;
  player.maxHp = 10;
  player.mp = 10;
  player.maxMp = 10;
  player.attack = 2;
  player.defense = 1;
  player.gold = 200;

  // Masukkan pointer player ke GameContext
  GameContext context;
  context.player = &player;

  SceneManager sceneManager(context);

  while (!WindowShouldClose() && !sceneManager.ShouldClose()) {
    float dt = GetFrameTime();
    sceneManager.Update(dt);

    BeginDrawing();
    ClearBackground(BLACK);
    sceneManager.Draw();
    EndDrawing();
  }

  CloseAudioDevice();
  CloseWindow();
  return 0;
}
