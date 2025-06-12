#include <stdexcept>
#include "Player.hpp"
#include "Game.hpp"
#include <iostream>
using namespace std;

namespace coup
{

   int Player::nextId = 1;

   Player::Player(Game &game) : game(game), playercoins(0), id(nextId++)
   {
      game.addPlayer(this);
   }

   Player::~Player() {

   }
    const int Player::getId() const{
      return id;
    }

   void Player::addcoins(int sum)
   {
      playercoins += sum;
   }

   void Player::subcoins(int sum)
   {
      playercoins -= sum;
   }

   void Player::iscorrectTurn() 
   {
      if (this != game.getCurrentPlayer()) {
        throw std::runtime_error("Not your turn");
    }
      
   }

   void Player::gather()
   {
      if (isSanction)
      {
         throw std::runtime_error("You are under sanction and cannot gather.");
      }
      iscorrectTurn();
      addcoins(1);
      lastAction = "gather";
      game.nextTurn(*this);
   }

   void Player::tax()
   {
      if (isSanction)
      {
         throw std::runtime_error("You are under sanction and cannot collect tax.");
      }
      iscorrectTurn();
      addcoins(2);
      lastAction = "tax";
      game.nextTurn(*this);
   }

   void Player::bribe()
   {
      iscorrectTurn();
      if (playercoins < 4)
      {
         throw std::runtime_error("Not enough coins");
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
      if (&p == lastArrested) {
        throw std::runtime_error("Cannot arrest the same player twice in a row");
      }
      if (p.playercoins == 0)
      {
         throw std::runtime_error("The player you selected doesnt have enough coins");
      }
      if (p.role() == "Merchant"){
         p.subcoins(2);
         lastAction = "arrest";
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
      lastArrested = &p;
      game.nextTurn(*this);
   }

   void Player::sanction(Player &p)
   {
      iscorrectTurn();
      if (playercoins < 3)
      {
         throw std::runtime_error("Not enough coins to use sanction.");
      }
      subcoins(3);
      p.isSanction = true;
      lastAction = "sanction";
      if (p.role() == "Judge"){
         subcoins(1);
      }
      if (p.role() == "Baron"){
         p.addcoins(1);
      }
      
      game.nextTurn(*this);
   }

   void Player::coup(Player &p)
   {
      iscorrectTurn();
      if (playercoins < 7)
      {
         throw std::runtime_error("You need at least 7 coins to perform a coup.");
      }
      subcoins(7);
      game.eliminatePlayer(p);
      lastAction = "coup";
      game.nextTurn(*this);
   }

   int Player::coins() const
   {
      return playercoins;
   }

   void Player::block(Player &p)
   {
      throw std::runtime_error(role() + " cannot block any action");
   }

   const std::string &Player::getLastAction() const
   {
      return lastAction;
   }

   const bool Player::getlastActionBlocked() const
   {
      return lastActionBlocked;
   }

   const void Player::setlastActionBlocked(bool b)
   {
      lastActionBlocked = b;
   }

   const void Player::setArrestIsBlock(bool b)
   {
      arrestIsBlock = b;
   }
   void Player::subBribrCount()
   {
      bribeCount -= 1;
   }
   std::string Player::getLastAction(){
      return lastAction;

   }

}
