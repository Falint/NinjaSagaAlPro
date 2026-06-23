#include "core/GameUtils.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>

namespace GameUtils {

  void SaveGame(const Character& player, const std::string& filepath, SaveCallback onSuccess) {
    std::ofstream outFile(filepath);
    if (!outFile.is_open()) {
      throw std::runtime_error("Gagal membuka file save untuk menulis: " + filepath);
    }

    // Tulis stats dasar
    outFile << player.name << "\n";
    outFile << player.hp << " " << player.maxHp << "\n";
    outFile << player.mp << " " << player.maxMp << "\n";
    outFile << player.attack << " " << player.defense << "\n";
    outFile << player.gold << "\n";

    // Tulis daftar skill
    outFile << player.skills.size() << "\n";
    for (const auto& skill : player.skills) {
      outFile << skill.id << "|" << skill.name << "|" << skill.manaCost << "|" << skill.baseDamage << "|" << skill.healAmount << "|" << static_cast<int>(skill.target) << "\n";
    }

    outFile.close();
    if (onSuccess) {
      onSuccess("Game successfully saved to " + filepath);
    }
  }

  void LoadGame(Character& player, const std::string& filepath, SaveCallback onSuccess) {
    std::ifstream inFile(filepath);
    if (!inFile.is_open()) {
      throw std::runtime_error("Gagal membuka file save untuk membaca: " + filepath);
    }

    std::string name;
    if (!std::getline(inFile, name)) {
      throw std::runtime_error("Format file save tidak valid (gagal membaca nama)");
    }
    player.name = name;

    if (!(inFile >> player.hp >> player.maxHp)) {
      throw std::runtime_error("Format file save tidak valid (gagal membaca HP)");
    }
    if (!(inFile >> player.mp >> player.maxMp)) {
      throw std::runtime_error("Format file save tidak valid (gagal membaca MP)");
    }
    if (!(inFile >> player.attack >> player.defense)) {
      throw std::runtime_error("Format file save tidak valid (gagal membaca stats)");
    }
    if (!(inFile >> player.gold)) {
      throw std::runtime_error("Format file save tidak valid (gagal membaca gold)");
    }

    size_t skillCount = 0;
    if (!(inFile >> skillCount)) {
      throw std::runtime_error("Format file save tidak valid (gagal membaca jumlah skill)");
    }

    std::string dummy;
    std::getline(inFile, dummy); // Konsumsi sisa newline

    player.skills.clear();
    for (size_t i = 0; i < skillCount; ++i) {
      std::string line;
      if (!std::getline(inFile, line)) {
        throw std::runtime_error("Format file save tidak valid (gagal membaca skill ke-" + std::to_string(i) + ")");
      }

      // Parsing manual string terpisah '|'
      size_t pos = 0;
      std::vector<std::string> tokens;
      while ((pos = line.find('|')) != std::string::npos) {
        tokens.push_back(line.substr(0, pos));
        line.erase(0, pos + 1);
      }
      tokens.push_back(line);

      if (tokens.size() >= 6) {
        Skill skill;
        skill.id = std::stoi(tokens[0]);
        skill.name = tokens[1];
        skill.manaCost = std::stoi(tokens[2]);
        skill.baseDamage = std::stoi(tokens[3]);
        skill.healAmount = std::stoi(tokens[4]);
        skill.target = static_cast<SkillTarget>(std::stoi(tokens[5]));
        
        bool isDuplicate = false;
        for (const auto& existingSkill : player.skills) {
          if (existingSkill.id == skill.id) {
            isDuplicate = true;
            break;
          }
        }
        
        if (!isDuplicate) {
          player.skills.push_back(skill);
        }
      } else if (tokens.size() == 5) {
        // Backward compatibility for old save format without ID
        Skill skill;
        skill.name = tokens[0];
        skill.manaCost = std::stoi(tokens[1]);
        skill.baseDamage = std::stoi(tokens[2]);
        skill.healAmount = std::stoi(tokens[3]);
        skill.target = static_cast<SkillTarget>(std::stoi(tokens[4]));
        
        // Recover ID based on name for legacy saves
        if (skill.name == "Fire Slash") skill.id = 1;
        else if (skill.name == "Ice Slash") skill.id = 2;
        else if (skill.name == "Heal") skill.id = 3;
        
        bool isDuplicate = false;
        for (const auto& existingSkill : player.skills) {
          if (existingSkill.id == skill.id) {
            isDuplicate = true;
            break;
          }
        }
        
        if (!isDuplicate) {
          player.skills.push_back(skill);
        }
      }
    }

    inFile.close();
    if (onSuccess) {
      onSuccess("Game successfully loaded from " + filepath);
    }
  }

  void SortSkills(Character& player) {
    // Fulfills: Sort + Lambda
    std::sort(player.skills.begin(), player.skills.end(), [](const Skill& a, const Skill& b) {
      return a.manaCost < b.manaCost; // Urutkan berdasarkan mana cost terendah
    });
  }

  bool HasSkill(const Character& player, const std::string& skillName) {
    // Fulfills: Find + Lambda
    auto it = std::find_if(player.skills.begin(), player.skills.end(), [&skillName](const Skill& s) {
      return s.name == skillName;
    });
    return it != player.skills.end();
  }

  int CountSkillsWithManaCostLessThan(const Character& player, int maxCost) {
    // Fulfills: Count + Lambda
    return std::count_if(player.skills.begin(), player.skills.end(), [maxCost](const Skill& s) {
      return s.manaCost < maxCost;
    });
  }
}
