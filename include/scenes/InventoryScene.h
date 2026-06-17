#pragma once

#include "core/BaseScene.h"
#include "types.h"

class InventoryScene : public BaseScene {
public:
  explicit InventoryScene(GameContext context);
  ~InventoryScene() override = default;

  void OnEnter() override;
  SceneType Update(float dt) override;
  void Draw() override;
  void OnExit() override;

private:
  // Variabel untuk animasi karakter idle_down.png
  Texture2D charTexture_ = {};     // Texture animasi karakter
  int charFramesCounter_ = 0;      // Penghitung waktu untuk ganti frame
  int charCurrentFrame_ = 0;       // Menyimpan frame aktif saat ini
  int charNumFrames_ = 8;          // Jumlah total frame dalam sprite (768px / 96px = 8)
  
  GameContext context_;
  Texture2D invTexture_ = {}; // Inv.png — background frame inventory
  Texture2D hitboxTexture_ =
      {}; // Inv_hitbox.png — overlay yang berpindah ke slot hover
  int hoveredSlot_ = -1; // -1 = tidak ada hover, 0-8 = index slot
};
