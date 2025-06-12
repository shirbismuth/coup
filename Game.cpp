#include "Game.hpp"
#include <stdexcept>
#include <iostream>
#include <algorithm>

using namespace std;

namespace coup
{

    Game::Game() {};
    Game::~Game()
    {
        for (Player *p : players)
        {
            delete p;
        }
        players.clear();
    };

    void Game::addPlayer(Player *player)
    {
        players.push_back(player);
    }

    void Game::nextTurn(Player &p)
    {
        p.isSanction = false;
        if (p.bribeCount > 0)
        {
            p.bribeCount--;
            return;
        }
        currentTurn++;
    }
    Player *Game::getCurrentPlayer()
    {
        return players[currentTurn % players.size()];
    }

    void Game::eliminatePlayer(Player &p)
    {
        auto it = std::find(players.begin(), players.end(), &p);
        if (it != players.end())
        {
            eliminatedPlayers.push_back(&p); // Add to eliminated list
            lastcoup = &p;
            players.erase(it);
            if ((it - players.begin()) <= currentTurn && currentTurn > 0)
            {
                currentTurn--;
            }
        }
    }

    std::string Game::createRandomPlayer()
    {
        if (!initialized)
        {
            srand(time(nullptr)); // אתחול חד־פעמי
            initialized = true;
        }
        int r = rand() % 6;
        Player *p = nullptr;

        switch (r)
        {
        case 0:
            p = new Merchant(*this);
            break;
        case 1:
            p = new Spy(*this);
            break;
        case 2:
            p = new General(*this);
            break;
        case 3:
            p = new Judge(*this);
            break;
        case 4:
            p = new Baron(*this);
            break;
        case 5:
            p = new Governor(*this);
            break;
        }

        if (p)
        {
            return p->role();
        }

        throw std::runtime_error("Failed to create player");
    }

    void Game::printplayer()
    {
        for (size_t i = 0; i < players.size(); ++i)
        {
            std::cout << "Player " << i + 1 << ": " << players[i]->role() << std::endl;
        }
    }

    std::vector<Player *> &Game::getPlayers()
    {
        return players;
    }

    Player *Game::getNextPlayer()
    {
        if (players.empty())
        {
            throw std::runtime_error("No players in the game.");
        }
        int nextIndex = (currentTurn + 1) % players.size();
        return players[nextIndex];
    }

    bool Game::isEliminated(Player *p) const
    {
        return std::find(eliminatedPlayers.begin(), eliminatedPlayers.end(), p) != eliminatedPlayers.end();
    }
    std::vector<Player *> Game::getEliminatedPlayers() const
    {
        return eliminatedPlayers;
    }

}