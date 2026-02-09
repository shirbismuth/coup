#include "../include/Game.hpp"
#include <stdexcept>
#include <iostream>
#include <algorithm>

using namespace std;

namespace coup
{

    Game::Game()
    {
        Player::resetIdCounter();
        players.clear();
        eliminatedPlayers.clear();
    };
    Game::~Game()
    {
    };

    void Game::addPlayer(Player *player)
    {
        if (players.size() >= 6)
        {
            throw std::runtime_error("Cannot add more than 6 players to the game.");
        }
        players.push_back(player);
    }

    // void Game::nextTurn(Player &p)
    // {
    //     p.isSanction = false;
    //     p.arrestIsBlock = false;
    //     if (p.bribeCount > 0)
    //     {
    //         p.bribeCount--;
    //         return;
    //     }
    //     currentTurn++;
    // }

    void Game::nextTurn(Player &p)
    {
        p.isSanction = false;
        p.arrestIsBlock = false;

        if (p.bribeCount > 0)
        {
            p.bribeCount--;
            return;
        }

        // מצא את p מחדש ברשימת השחקנים (שמא התעדכנה)
        auto it = std::find(players.begin(), players.end(), &p);
        if (it != players.end())
        {
            size_t index = std::distance(players.begin(), it);
            currentTurn = (index + 1) % players.size(); // מעבירים תור לשחקן הבא
        }
        else
        {
            currentTurn = 0; // fallback
        }
    }

    Player *Game::getCurrentPlayer()
    {
        if (players.empty())
        {
            throw std::runtime_error("No active players in the game.");
        }
        return players[currentTurn % players.size()];
    }

    void Game::eliminatePlayer(Player &p)
    {
        // שמירה על השחקן שהיה בתור לפני המחיקה
        Player *current = players[currentTurn];

        // מציאת השחקן שצריך להימחק
        auto it = std::find(players.begin(), players.end(), &p);
        if (it != players.end())
        {
            eliminatedPlayers.push_back(&p);
            lastcoup = &p;
            players.erase(it);

            // מציאת מיקום חדש של השחקן שבתור
            auto newIt = std::find(players.begin(), players.end(), current);
            if (newIt != players.end())
            {
                currentTurn = std::distance(players.begin(), newIt);
            }
            else
            {
                currentTurn = 0; // fallback – לא סביר, אבל ליתר ביטחון
            }
        }
    }

    void Game::removefromeliminatePlayer(Player &p)
    {
        auto it = std::find(eliminatedPlayers.begin(), eliminatedPlayers.end(), &p);
        if (it != eliminatedPlayers.end())
        {
            eliminatedPlayers.erase(it);
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
    Player *Game::getLastArrested()
    {
        return lastArrested;
    }
    void Game::setLastArrested(Player *p)
    {
        lastArrested = p;
    }

    bool Game::isPlayerInGame(Player *p) const
    {
        for (const auto &player : players)
        {
            if (player == p && !player->eliminated)
            {
                return true;
            }
        }
        return false;
    }
    void Game::checkwinner()
    {
        if (players.size() == 1)
        {
            winner = players[0];
            gameOver = true;
        }
    }

    bool Game::getgameover()
    {
        return gameOver;
    }

    Player *Game::getwinner()
    {
        return winner;
    }

    Player *Game::getLastCoup() const
    {

        if (!eliminatedPlayers.empty())
        {
            return eliminatedPlayers.back();
        }
        return nullptr;
    }
}
