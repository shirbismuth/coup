#pragma once
#include "Player.hpp"
#include "Game.hpp"

namespace coup {

class Baron : public Player {
public:
    Baron(Game& game);
    void invest(); 
    std::string role() const override;
};
}