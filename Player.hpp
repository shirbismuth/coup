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
    int coins;
    int id;
    static int nextId; 
    std::string lastAction ="";    
    bool lastActionBlocked = false;
    bool arrestIsBlock = false;
    bool isSanction = false;
    Player* lastArrested = nullptr;
    int bribeCount =0;
    bool eliminated = false;
    bool eliminatedinthisturn = false;
    
 


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
    static void resetIdCounter() { nextId = 1; }
    void iscorrectTurn();
    int getcoins() const;
    virtual std::string role() const = 0;
    const void setArrestIsBlock(bool b);
    void subBribrCount();
    std::string getLastAction();
    bool geteliminated();
    void seteliminated(bool b);
    

    
    



};

}
#endif
    
