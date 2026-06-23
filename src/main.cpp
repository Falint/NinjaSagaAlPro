#include "core/SceneManager.h"
#include "core/GameUtils.h"
#include "entities/Character.h"
#include "raylib.h"
#include "types.h"
#include <iostream>
#include <stdexcept>

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

  // === Demonstrasi File Handling, Callback, dan Exception Handling ===
  std::string savePath = "savegame.txt";
  auto onIoSuccess = [](const std::string& message) {
    std::cout << "[CALLBACK SUCCESS] " << message << std::endl;
  };

  try {
    std::cout << "Mencoba memuat data save game..." << std::endl;
    GameUtils::LoadGame(player, savePath, onIoSuccess);
  } catch (const std::exception& e) {
    std::cerr << "[EXCEPTION CAUGHT] " << e.what() << std::endl;
    std::cout << "Membuat data save game baru..." << std::endl;
    try {
      GameUtils::SaveGame(player, savePath, onIoSuccess);
    } catch (const std::exception& saveError) {
      std::cerr << "[EXCEPTION CAUGHT] Gagal menyimpan game baru: " << saveError.what() << std::endl;
    }
  }

  // === Demonstrasi Overloading Operator + ===
  player = player + 50; // Menambahkan 50 gold menggunakan operator+ yang di-overload
  std::cout << "Gold setelah ditambah 50 (overloading operator+): " << player.gold << std::endl;

  // === Demonstrasi STL Algorithms (Sort, Find, Count) dengan Lambda ===
  GameUtils::SortSkills(player);
  bool hasHeal = GameUtils::HasSkill(player, "Fire Spark");
  int skillCountCheap = GameUtils::CountSkillsWithManaCostLessThan(player, 6);
  std::cout << "Apakah memiliki skill Fire Spark (Find): " << std::boolalpha << hasHeal << std::endl;
  std::cout << "Jumlah skill dengan mana < 6 (Count): " << skillCountCheap << std::endl;

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
