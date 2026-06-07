#include <iostream>
#include "raylib.h"
using namespace std;

int main() {
    cout << "Hello, Ninja Saga!" << endl;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 720, "Ninja Saga");
    SetTargetFPS(60);
    InitAudioDevice();
  

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            EndDrawing();
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}