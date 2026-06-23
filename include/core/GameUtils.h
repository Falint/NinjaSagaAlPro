#pragma once
#include "entities/Character.h"
#include <functional>
#include <string>

namespace GameUtils {
  // Callback type
  using SaveCallback = std::function<void(const std::string&)>;

  // File Handling & Exception
  void SaveGame(const Character& player, const std::string& filepath, SaveCallback onSuccess);
  void LoadGame(Character& player, const std::string& filepath, SaveCallback onSuccess);

  // STL algorithms: Sort, Find, Count with Lambda
  void SortSkills(Character& player);
  bool HasSkill(const Character& player, const std::string& skillName);
  int CountSkillsWithManaCostLessThan(const Character& player, int maxCost);
}
