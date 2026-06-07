#include <iostream>
#include "raylib.h"
using namespace std;

int main() {
    cout << "Hello, Raylib!" << endl;
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