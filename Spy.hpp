#pragma once
#include "Player.hpp"

namespace coup {

class Spy : public Player {
public:
    Spy(Game& game);
    int showcoins(const Player& p);
    void block(Player& p) override;
    std::string role() const override;
};

}