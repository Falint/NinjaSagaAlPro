#include "Combat.h"
#include <iostream>
#include <limits>

float Combat::getElementalMultiplier(Element attacker, Element defender) {
    if (attacker == Element::NEUTRAL || defender == Element::NEUTRAL) return 1.0f;
    
    if (attacker == Element::FIRE && defender == Element::WIND) return 1.5f;
    if (attacker == Element::WIND && defender == Element::EARTH) return 1.5f;
    if (attacker == Element::EARTH && defender == Element::WATER) return 1.5f;
    if (attacker == Element::WATER && defender == Element::FIRE) return 1.5f;
    
    if (attacker == Element::WIND && defender == Element::FIRE) return 0.5f;
    if (attacker == Element::EARTH && defender == Element::WIND) return 0.5f;
    if (attacker == Element::WATER && defender == Element::EARTH) return 0.5f;
    if (attacker == Element::FIRE && defender == Element::WATER) return 0.5f;
    
    if (attacker == defender) return 0.75f;
    
    return 1.0f;
}

void Combat::combatLoop(Player* p, Enemy* e) {
    std::cout << "\n--- COMBAT START ---\n";
    std::cout << "A wild Lvl " << e->level << " enemy appears! Element: " << e->getElementName() << "\n";
    
    int turn = 1;
    while (p->hp > 0 && e->hp > 0) {
        std::cout << "\n[Turn " << turn << "]\n";
        std::cout << "Player: " << p->hp << "/" << p->maxHp << " HP | "
                  << "Enemy: " << e->hp << "/" << e->maxHp << " HP\n";
        
        std::cout << "1. Attack\nChoice: ";
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = 1;
        }
        
        if (choice == 1) {
            float mult = getElementalMultiplier(p->element, e->element);
            int damage = static_cast<int>(p->baseAttack * mult);
            e->hp -= damage;
            std::cout << "> Player attacks! Deals " << damage << " damage (Multiplier: " << mult << "x)\n";
        } else {
            std::cout << "> Player did nothing!\n";
        }
        
        if (e->hp <= 0) {
            std::cout << "\nEnemy defeated! You win!\n";
            break;
        }
        
        float eMult = getElementalMultiplier(e->element, p->element);
        int eDamage = static_cast<int>(e->baseAttack * eMult);
        p->hp -= eDamage;
        std::cout << "> Enemy attacks! Deals " << eDamage << " damage (Multiplier: " << eMult << "x)\n";
        
        if (p->hp <= 0) {
            std::cout << "\nYou were defeated...\n";
            break;
        }
        
        turn++;
    }
    std::cout << "--- COMBAT END ---\n\n";
}
