#pragma once
#include "Player.hpp"
#include "Game.hpp"
#include <iostream>

namespace coup {

class Governor : public Player {
public:
    Governor(Game& game);
    void tax() override;
    void cancelTax(Player& p);
    std::string role() const override;
};

}
