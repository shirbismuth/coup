#include "Judge.hpp"
#include <stdexcept>
#include <iostream>

namespace coup
{

    Judge::Judge(Game &game) : Player(game) {
    }

    void Judge::cancelBribe()
    {
        std::cout << "try cancel bribe" << std::endl;
        if (game.getCurrentPlayer()->getLastAction() == "bribe")
        {
            std::cout << "cancel bribe" << std::endl;
            game.getCurrentPlayer()->subBribrCount();
        }
        else
        {
            throw std::runtime_error("Illegal action");
        }
    }

    std::string Judge::role() const
    {
        return "Judge";
    }

}