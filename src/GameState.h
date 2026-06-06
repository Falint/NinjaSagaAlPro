#pragma once
#include "Entity.h"

enum class State {
    MAIN_MENU,
    TOWN_SHOP,
    EXPLORATION,
    COMBAT,
    EXIT
};

class GameState {
public:
    State currentState;
    Player* player;

    GameState();
    ~GameState();

    void handleState();
};
