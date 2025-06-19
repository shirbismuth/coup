#include "Governor.hpp"

namespace coup{
    Governor::Governor(Game& game) : Player(game){}

    void Governor::tax(){
        iscorrectTurn();
      if (coins >=10)
      {
         throw std::runtime_error("You have 10+ coins. You must perform a coup.");
      }
      if (isSanction)
      {
         throw std::runtime_error("You are under sanction and cannot collect tax.");
      }
      addcoins(3);
      lastAction = "tax";
      game.nextTurn(*this);
   }

   void Governor::cancelTax(Player& p){
    if (p.getLastAction() == "tax"){
        p.subcoins(2);
        std::cout << "cancel tax" << std::endl;
    }
    else { throw std::runtime_error("This player's last action was not tax.");} 
    
   }

   std::string Governor::role() const{
    return "Governor";
   }   


}
