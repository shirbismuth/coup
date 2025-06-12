#pragma once
#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <string>
using namespace std;

namespace coup {
    class Game;

class Player
{
    friend class Game;
protected:
    Game& game;
    int playercoins;
    int id;
    static int nextId; 
    std::string lastAction ="";    
    bool lastActionBlocked = false;
    bool arrestIsBlock = false;
    bool isSanction = false;
    Player* lastArrested = nullptr;
    int bribeCount =0;


public:
    Player(Game& game);
    virtual ~Player();
    virtual void gather();
    virtual void tax();
    virtual void bribe();
    virtual void arrest(Player& p);
    virtual void sanction(Player& P);
    virtual void coup(Player& p);
    void addcoins(int sum);
    void subcoins(int sum);
    const int getId() const;
    void iscorrectTurn();
    int coins() const;
    virtual void block(Player& p);
    virtual std::string role() const = 0;
    bool canact();
    const std::string& getLastAction() const;
    const bool getlastActionBlocked() const;
    const void setlastActionBlocked(bool b);
    const void setArrestIsBlock(bool b);
    void subBribrCount();
    std::string getLastAction();
    
    



};

}
#endif
    
