#include <iostream>
#include "raylib.h"
using namespace std;

int main() {
    cout << "Hello, Ninja Saga!" << endl;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 720, "Ninja Saga");
    SetTargetFPS(60);
    InitAudioDevice();
    Music bgm = LoadMusicStream("assets/audio/soundtrack/MiniMacro Sound - The Ninja Gaiden Soundtrack as Interpreted by MiniMacro Sound - 04 Vow of Revenge (Opening II).mp3");
    PlayMusicStream(bgm);
    
    while (!WindowShouldClose()) {
        UpdateMusicStream(bgm);
        BeginDrawing();
            ClearBackground(BLACK);
            EndDrawing();
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}