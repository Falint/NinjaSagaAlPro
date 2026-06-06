#include "GameState.h"
#include "Combat.h"
#include <iostream>
#include <limits>
#include <cstdlib>

GameState::GameState() : currentState(State::MAIN_MENU), player(nullptr) {}

GameState::~GameState() {
    delete player;
}

void GameState::handleState() {
    int choice;
    
    switch (currentState) {
        case State::MAIN_MENU:
            std::cout << "=== MAIN MENU ===\n";
            std::cout << "1. New Game\n2. Exit\nChoice: ";
            if (!(std::cin >> choice)) { 
                std::cin.clear(); 
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                choice = 2; 
            }
            
            if (choice == 1) {
                delete player;
                player = new Player(1, Element::NEUTRAL);
                std::cout << "\nNew game started!\n\n";
                currentState = State::TOWN_SHOP;
            } else {
                currentState = State::EXIT;
            }
            break;
            
        case State::TOWN_SHOP:
            std::cout << "=== TOWN ===\n";
            std::cout << "1. Rest (Heal to Full)\n2. Go Explore\n3. Main Menu\nChoice: ";
            if (!(std::cin >> choice)) { 
                std::cin.clear(); 
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                choice = 3; 
            }
            
            if (choice == 1 && player) {
                player->hp = player->maxHp;
                std::cout << "\nYou rested and fully recovered your HP.\n\n";
            } else if (choice == 2) {
                currentState = State::EXPLORATION;
                std::cout << "\n";
            } else if (choice == 3) {
                currentState = State::MAIN_MENU;
                std::cout << "\n";
            }
            break;
            
        case State::EXPLORATION:
            std::cout << "=== EXPLORATION ===\n";
            std::cout << "1. Walk around\n2. Return to Town\nChoice: ";
            if (!(std::cin >> choice)) { 
                std::cin.clear(); 
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                choice = 2; 
            }
            
            if (choice == 1) {
                if (std::rand() % 2 == 1) { // 50% encounter rate
                    std::cout << "\nAn enemy suddenly attacks!\n";
                    currentState = State::COMBAT;
                } else {
                    std::cout << "\nYou explored but nothing happened.\n\n";
                }
            } else if (choice == 2) {
                currentState = State::TOWN_SHOP;
                std::cout << "\n";
            }
            break;
            
        case State::COMBAT:
            if (player) {
                // Dynamically generate enemy
                Enemy* enemy = new Enemy(player->level);
                
                Combat::combatLoop(player, enemy);
                
                // Memory management: delete dynamically allocated enemy
                delete enemy;
                
                if (player->hp <= 0) {
                    std::cout << "Game Over! Returning to Main Menu...\n\n";
                    currentState = State::MAIN_MENU;
                } else {
                    std::cout << "Returning to exploration...\n\n";
                    player->level++;
                    player->baseAttack += 5;
                    player->maxHp += 20;
                    player->hp = player->maxHp; // Heal to full on level up
                    std::cout << "Level Up! You are now Lvl " << player->level << ".\n\n";
                    currentState = State::EXPLORATION;
                }
            } else {
                currentState = State::MAIN_MENU;
            }
            break;
            
        case State::EXIT:
            break;
    }
}
