#pragma once
#include "core/BaseScene.h"
#include "items/Item.h"
#include "types.h"
#include <vector>

class ShopScene : public BaseScene {
public:
  explicit ShopScene(GameContext ctx);
  ~ShopScene() override = default;

  void OnEnter() override;
  SceneType Update(float dt) override;
  void Draw() override;
  void OnExit() override;

private:
  GameContext context_;

  // Daftar item yang dijual di shop
  std::vector<Item> shopItems_;

  // UI state
  int selectedIndex_ = -1; // Item yang sedang di-hover/pilih
  int currentPage_ = 0;    // Halaman aktif (untuk pagination)

  // TODO: Nanti sambungkan ke player inventory & gold
  // Character* playerRef_;
};
