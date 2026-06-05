#pragma once
#include "Entity.h"

class Combat {
public:
    static float getElementalMultiplier(Element attacker, Element defender);
    static void combatLoop(Player* player, Enemy* enemy);
};
