#pragma once
#include "Player.hpp"
#include "Game.hpp"

namespace coup {

class Judge : public Player {
public:
    Judge(Game& game);
    void cancelBribe(); 
    std::string role() const override;
};
}
