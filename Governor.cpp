#include "Governor.hpp"

namespace coup{
    Governor::Governor(Game& game) : Player(game){}

    void Governor::tax(){
      iscorrectTurn();
      addcoins(3);
      game.nextTurn(*this);
   }

   void Governor::cancelTax(Player& p){
    if (p.getLastAction() == "tax"){
        p.subcoins(2);
        std::cout << "cancel tax" << std::endl;
    }
    else { std::cout << "This player's last action was not tax." << std::endl;} 
    
   }

   std::string Governor::role() const{
    return "Governor";
   }   


}
