#include "core/SceneManager.h"
#include "raylib.h"
#include "types.h"
#include <iostream>

using namespace std;

int main() {
  cout << "Starting Ninja Saga..." << endl;
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1200, 720, "Ninja Saga");
  SetExitKey(KEY_NULL);
  SetTargetFPS(60);
  InitAudioDevice();

  GameContext context;
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
