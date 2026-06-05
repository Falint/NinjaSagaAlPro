#include "Entity.h"
#include <cstdlib>
#include <algorithm>

Entity::Entity(int level, Element el) : level(level), element(el) {}

std::string Entity::getElementName() const {
    switch (element) {
        case Element::FIRE: return "Fire";
        case Element::WATER: return "Water";
        case Element::EARTH: return "Earth";
        case Element::WIND: return "Wind";
        default: return "Neutral";
    }
}

Player::Player(int level, Element el) : Entity(level, el) {
    maxHp = 100 + (level * 20);
    hp = maxHp;
    maxMp = 50 + (level * 10);
    mp = maxMp;
    baseAttack = 10 + (level * 5);
}

Enemy::Enemy(int playerLevel) : Entity(1, Element::NEUTRAL) {
    // Variance +/- 1 from player level
    int variance = (std::rand() % 3) - 1;
    level = std::max(1, playerLevel + variance);
    
    maxHp = 50 + (level * 15);
    hp = maxHp;
    baseAttack = 5 + (level * 4);
    
    // Random element from FIRE (1) to WIND (4)
    element = static_cast<Element>((std::rand() % 4) + 1);
}
