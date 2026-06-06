#pragma once
#include <string>

enum class Element {
    NEUTRAL,
    FIRE,
    WATER,
    EARTH,
    WIND
};

class Entity {
public:
    int hp;
    int maxHp;
    int baseAttack;
    int level;
    Element element;

    Entity(int level, Element el);
    virtual ~Entity() = default;
    
    std::string getElementName() const;
};

class Player : public Entity {
public:
    int mp;
    int maxMp;

    Player(int level, Element el);
};

class Enemy : public Entity {
public:
    Enemy(int playerLevel);
};
