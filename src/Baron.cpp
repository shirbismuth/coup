#include "../include/Baron.hpp"
#include <stdexcept> 

namespace coup{

    Baron::Baron(Game& game) : Player(game){}

    void Baron::invest(){
        iscorrectTurn();
        if (coins<3){
          throw std::runtime_error("You don't have enough coins.");
      }
      subcoins(3);
      addcoins(6);
      lastAction = "invest";
      game.nextTurn(*this);
    
    }

    std::string Baron::role() const{
    return "Baron";
   }   



}
