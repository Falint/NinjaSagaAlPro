#pragma once
#include <string>

// Kategori item — untuk filter tab di inventory/shop
enum class ItemCategory {
  Weapon,     // Senjata (pedang, kunai, shuriken)
  Armor,      // Pelindung (helm, baju besi, tameng)
  Consumable, // Bisa dipakai habis (potion, elixir, makanan)
  Material    // Bahan craft / quest item
};

// Data satu item
struct Item {
  int id = 0;                                    // ID unik item
  std::string name = "Unknown";                  // Nama item
  std::string description = "";                  // Deskripsi singkat
  ItemCategory category = ItemCategory::Consumable;
  int quantity = 1;                              // Jumlah yang dimiliki
  int buyPrice = 0;                              // Harga beli di shop
  int sellPrice = 0;                             // Harga jual

  // === Stat bonuses (untuk equipment) ===
  int attackBonus = 0;
  int defenseBonus = 0;
  int hpBonus = 0;

  // === Efek konsumsi (untuk consumable) ===
  int healAmount = 0;      // HP yang di-restore
  int manaRestore = 0;     // MP yang di-restore
};
