#pragma once
#include "Player.hpp"

namespace coup {

class General : public Player {
public:
    General(Game& game);
    void cancelcoup(Player& p); 
    std::string role() const override;
};
}
