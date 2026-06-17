#include "scenes/ShopScene.h"
#include "constants.h"
#include "raylib.h"
#include <iostream>

// ─── Constructor ─────────────────────────────────────────────
ShopScene::ShopScene(GameContext ctx) : context_(ctx) {}

// ─── OnEnter ─────────────────────────────────────────────────
void ShopScene::OnEnter() {
  // TODO: Isi daftar item toko (nanti bisa load dari file/data)
  shopItems_ = {
      {1, "Health Potion", "Restores 5 HP", ItemCategory::Consumable, 1, 50, 25,
       0, 0, 0, 5, 0},
      {2, "Mana Potion", "Restores 3 MP", ItemCategory::Consumable, 1, 40, 20,
       0, 0, 0, 0, 3},
      {3, "Iron Sword", "A basic sword", ItemCategory::Weapon, 1, 100, 50, 3, 0,
       0, 0, 0},
      {4, "Wooden Shield", "A basic shield", ItemCategory::Armor, 1, 80, 40, 0,
       2, 0, 0, 0},
  };

  selectedIndex_ = -1;
  currentPage_ = 0;

  std::cout << "[ShopScene] Shop opened!" << std::endl;
}

// ─── Update ──────────────────────────────────────────────────
SceneType ShopScene::Update(float dt) {
  (void)dt;

  // Kembali ke Main Menu dengan ESC atau BACKSPACE
  if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) {
    return SceneType::MainMenu;
  }

  // TODO: Implementasi mouse hover detection pada daftar item
  // TODO: Implementasi klik untuk beli item
  // TODO: Pagination jika item toko banyak

  return SceneType::None;
}

// ─── Draw ────────────────────────────────────────────────────
void ShopScene::Draw() {
  ClearBackground(BLACK);

  // TODO: Draw background shop
  // TODO: Draw daftar item dengan harga
  // TODO: Draw gold player
  // TODO: Draw item yang di-hover/selected

  // Placeholder text
  const char *title = "SHOP - Coming Soon";
  int titleW = MeasureText(title, 40);
  DrawText(title, (GetScreenWidth() - titleW) / 2, GetScreenHeight() / 2 - 20,
           40, GOLD);

  DrawText("Press ESC to go back", 20, GetScreenHeight() - 40, 20, GRAY);
}

// ─── OnExit ──────────────────────────────────────────────────
void ShopScene::OnExit() {
  shopItems_.clear();
  selectedIndex_ = -1;
  currentPage_ = 0;
  std::cout << "[ShopScene] Shop closed." << std::endl;
}
