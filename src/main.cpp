#include "GameState.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

int main() {
    // Seed for random number generation
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    std::cout << "Starting C++ RPG Engine...\n\n";
    
    // Initialize state machine
    GameState game;
    
    // Main Game Loop
    while (game.currentState != State::EXIT) {
        game.handleState();
    }
    
    std::cout << "Exiting game...\n";
    return 0;
}
