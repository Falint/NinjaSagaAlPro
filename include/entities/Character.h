#pragma once
#include "entities/Skill.h"
#include "items/Item.h"
#include <string>
#include <vector>

// Data karakter — dipakai untuk Player DAN Enemy
struct Character {
  std::string name = "Ninja";

  // === Base Stats ===
  int hp = 10;
  int maxHp = 10;
  int mp = 5;
  int maxMp = 5;
  int attack = 2;
  int defense = 1;
  int gold = 100; // Uang (khusus player)

  // === Skills & Inventory ===
  std::vector<Skill> skills;   // Daftar skill yang dikuasai
  std::vector<Item> inventory; // Item yang dibawa

  // === Helper Methods ===

  // Apakah karakter masih hidup?
  bool IsAlive() const { return hp > 0; }

  // Overloading operator + untuk menambah gold ke Character dengan mudah
  Character operator+(int goldAmount) const {
    Character temp = *this;
    temp.gold += goldAmount;
    return temp;
  }

  // Terima damage (dikurangi defense)
  void TakeDamage(int rawDamage) {
    int actualDamage = rawDamage - defense;
    if (actualDamage < 1) actualDamage = 1; // Minimal 1 damage
    hp -= actualDamage;
    if (hp < 0) hp = 0;
  }

  // Overloading method TakeDamage (bisa ignore defense)
  void TakeDamage(int rawDamage, bool ignoreDefense) {
    if (ignoreDefense) {
      hp -= rawDamage;
      if (hp < 0) hp = 0;
    } else {
      TakeDamage(rawDamage);
    }
  }

  // Heal HP (tidak boleh melebihi maxHp)
  void Heal(int amount) {
    hp += amount;
    if (hp > maxHp) hp = maxHp;
  }

  // Restore MP (tidak boleh melebihi maxMp)
  void RestoreMana(int amount) {
    mp += amount;
    if (mp > maxMp) mp = maxMp;
  }

  // Pakai mana untuk skill
  bool UseMana(int cost) {
    if (mp < cost) return false;
    mp -= cost;
    return true;
  }
};
