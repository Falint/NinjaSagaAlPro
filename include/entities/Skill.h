#pragma once
#include <string>

// Tipe target skill
enum class SkillTarget {
  SingleEnemy, // Serang 1 musuh
  AllEnemies,  // Serang semua musuh (AOE)
  Self,        // Buff diri sendiri
  SingleAlly   // Heal/buff teman (kalau ada party)
};

// Data satu skill
struct Skill {
  int id = 0;
  std::string name = "Basic Attack";
  std::string description = "";
  int manaCost = 0;       // MP yang dibutuhkan
  int baseDamage = 0;     // Damage dasar (sebelum dikalikan ATK)
  int healAmount = 0;     // Untuk skill heal
  SkillTarget target = SkillTarget::SingleEnemy;

  // -- Fields untuk Academy Shop --
  int price = 0;                // Harga beli (dalam Gold)
  bool owned = false;           // Apakah sudah dibeli?
  const char *iconPath = "";    // Path ke file icon PNG

  // Cek apakah skill bisa dipakai (cukup mana?)
  bool CanUse(int currentMana) const { return currentMana >= manaCost; }
};
