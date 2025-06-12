#include "General.hpp"
#include <stdexcept>
#include "Game.hpp"

namespace coup
{

    General::General(Game &game) : Player(game) {}

    void General::cancelcoup(Player &p)
    {
        if (playercoins < 5)
        {
            throw std::runtime_error("You don't have enough coins.");
        }
        subcoins(5);
        game.addPlayer(&p);
    }

    std::string General::role() const
    {
        return "General";
    }

}