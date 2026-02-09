#include "../include/General.hpp"
#include <stdexcept>
#include "../include/Game.hpp"

namespace coup
{

    General::General(Game &game) : Player(game) {}

    void General::cancelcoup(Player &p)
    {
        if (coins < 5)
        {
            throw std::runtime_error("You don't have enough coins.");
        }
        if (!p.geteliminated())
        {
            throw std::runtime_error("Player is not eliminated.");
        }
        subcoins(5);
        game.addPlayer(&p);
        game.removefromeliminatePlayer(p);
        p.seteliminated(false);
    }

    std::string General::role() const
    {
        return "General";
    }

}
