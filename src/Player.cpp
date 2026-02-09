#include <stdexcept>
#include "../include/Player.hpp"
#include "../include/Game.hpp"
#include <iostream>
using namespace std;

namespace coup
{

   int Player::nextId = 1;

   Player::Player(Game &game) : game(game), coins(0), id(nextId++)
   {
      game.addPlayer(this);
   }

   Player::~Player()
   {
   }
   const int Player::getId() const
   {
      return id;
   }

   void Player::addcoins(int sum)
   {
      coins += sum;
   }

   void Player::subcoins(int sum)
   {
      coins -= sum;
   }

   void Player::iscorrectTurn()
   {
      if (this != game.getCurrentPlayer())
      {
         throw std::runtime_error("Not your turn");
      }
   }

   void Player::gather()
   {
      iscorrectTurn();
      if (coins >=10)
      {
         throw std::runtime_error("You have 10+ coins. You must perform a coup.");
      }
      if (isSanction)
      {
         throw std::runtime_error("You are under sanction and cannot gather.");
      }
      addcoins(1);
      lastAction = "gather";
      game.nextTurn(*this);
   }

   void Player::tax()
   {
      iscorrectTurn();
      if (coins >=10)
      {
         throw std::runtime_error("You have 10+ coins. You must perform a coup.");
      }
      if (isSanction)
      {
         throw std::runtime_error("You are under sanction and cannot collect tax.");
      }
      addcoins(2);
      lastAction = "tax";
      game.nextTurn(*this);
   }

   void Player::bribe()
   {
      iscorrectTurn();
      if (coins >=10)
      {
         throw std::runtime_error("You have 10+ coins. You must perform a coup.");
      }
      if (coins < 4)
      {
         throw std::runtime_error("Not enough coins to bribe");
      }
      std::cout << "bribe" << std::endl;
      subcoins(4);
      bribeCount += 2;
      lastAction = "bribe";
      game.nextTurn(*this);
   }

   void Player::arrest(Player &p)
   {
      iscorrectTurn();
      if (coins >=10)
      {
         throw std::runtime_error("You have 10+ coins. You must perform a coup.");
      }
      if (arrestIsBlock == true)
      {
         throw std::runtime_error("The arrest action is blocked.");
      }
      if (&p == game.getLastArrested())
      {
         throw std::runtime_error("Cannot arrest the same player twice in a row");
      }
      if (p.coins == 0)
      {
         throw std::runtime_error("The player you selected doesnt have enough coins");
      }
      if (p.role() == "Merchant")
      {
         p.subcoins(2);
         lastAction = "arrest";
         game.setLastArrested(&p);
         game.nextTurn(*this);
         return;
      }
      p.subcoins(1);
      addcoins(1);
      if (p.role() == "General")
      {
         p.addcoins(1);
      }
      lastAction = "arrest";
      game.setLastArrested(&p);
      game.nextTurn(*this);
   }

   void Player::sanction(Player &p)
   {
      iscorrectTurn();
      if (coins >=10)
      {
         throw std::runtime_error("You have 10+ coins. You must perform a coup.");
      }
      if (coins < 3)
      {
         throw std::runtime_error("Not enough coins to use sanction.");
      }
      subcoins(3);
      p.isSanction = true;
      lastAction = "sanction";
      if (p.role() == "Judge")
      {
         subcoins(1);
      }
      if (p.role() == "Baron")
      {
         p.addcoins(1);
      }

      game.nextTurn(*this);
   }

   void Player::coup(Player &p)
   {
      iscorrectTurn();
      if ( this == &p){
          throw std::runtime_error("You cant coup yourself");
      }
      if (coins < 7)
      {
         throw std::runtime_error("You need at least 7 coins to perform a coup.");
      }
      if (!game.isPlayerInGame(&p))
      {
         throw std::runtime_error("Cannot perform coup on a player not in the game.");
      }
      if (p.eliminated)
      {
         throw std::runtime_error("Player already eliminated.");
      }

      subcoins(7);
      game.eliminatePlayer(p);
      lastAction = "coup";
      p.eliminated = true;
      game.checkwinner();
      game.nextTurn(*this);
   }

   int Player::getcoins() const
   {
      return coins;
   }

   const void Player::setArrestIsBlock(bool b)
   {
      arrestIsBlock = b;
   }
   void Player::subBribrCount()
   {
      bribeCount -= 1;
   }
   std::string Player::getLastAction()
   {
      return lastAction;
   }
   bool Player::geteliminated(){

      return eliminated;
   }
   void Player::seteliminated(bool b){
      eliminated = b;
   }

}
