#include "Spy.hpp"

namespace coup{

    Spy::Spy(Game& game) : Player(game){}

    int Spy::showcoins(const Player& p){
        return p.coins();
    }

    void Spy::block(Player& p){
        p.setArrestIsBlock(true);
    }


    std::string Spy::role() const{
    return "Spy";
   }   



}