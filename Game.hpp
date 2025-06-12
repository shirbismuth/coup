#pragma once
#include "Player.hpp"
#include "Merchant.hpp"
#include "Spy.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Baron.hpp"
#include "Governor.hpp"
#include <string>
#include <vector>


using namespace std;
namespace coup{

class Game{
private:

    std::vector<Player*> players;
    int currentTurn = 0;
    Player* lastcoup = nullptr;
    std::vector<Player *> eliminatedPlayers;
    bool initialized = false;


public:
    Game();
    ~Game();
    void addPlayer(Player* player);
    void nextTurn(Player& p);
    Player* getCurrentPlayer();
    void eliminatePlayer(Player& p);
    std::string createRandomPlayer();
    void printplayer();
    std::vector<Player*>& getPlayers();
    Player* getNextPlayer();
    bool isEliminated(Player* p) const;
    std::vector<Player*> getEliminatedPlayers() const;
    
};
}