#pragma once
#include "Player.hpp"
#include <iostream>

namespace coup {

class Spy : public Player {
public:
    Spy(Game& game);
    int showCoins(const Player& p);
    void blockArrest(Player& p);
    std::string role() const override;
};

}