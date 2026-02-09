#include "../include/Spy.hpp"

namespace coup{

    Spy::Spy(Game& game) : Player(game){}

    int Spy::showCoins(const Player& p){

        return p.getcoins();
    }

    void Spy::blockArrest(Player& p){
        std::cout << "Block arrest" << std::endl;
        p.setArrestIsBlock(true);
    }


    std::string Spy::role() const{
    return "Spy";
   }   



}
