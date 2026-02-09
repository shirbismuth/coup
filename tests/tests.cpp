#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../include/Game.hpp"
#include "../include/Player.hpp"
#include "../include/General.hpp"
#include "../include/Spy.hpp"
#include "../include/Judge.hpp"
#include "../include/Governor.hpp"
#include "../include/Baron.hpp"
#include "../include/Merchant.hpp"
#include <vector>
using namespace coup;

// Helper: cycle through all players in turn order and perform gather
void gather_round(std::vector<Player*>& players) {
    for (Player* p : players) {
        p->gather();
    }
}

TEST_CASE("Game setup and player limits") {
    Game game;
    Governor governor(game);
    Spy spy(game);
    Baron baron(game);
    General general(game);
    Judge judge(game);
    Merchant merchant(game);
    std::vector<Player*> players = game.getPlayers();
    CHECK(players.size() == 6);
    CHECK_THROWS_AS(game.addPlayer(new Merchant(game)), std::runtime_error);
}

TEST_CASE("Turn order enforcement and not your turn") {
    Game game;
    Governor governor(game);
    Spy spy(game);
    Baron baron(game);
    General general(game);
    Judge judge(game);
    Merchant merchant(game);
    std::vector<Player*> players = game.getPlayers();
    // Only the first player can act
    CHECK_NOTHROW(governor.gather());
    // Now it's spy's turn
    CHECK_THROWS_AS(governor.gather(), std::runtime_error);
    CHECK_NOTHROW(spy.gather());
    // Now it's baron's turn
    CHECK_THROWS_AS(governor.gather(), std::runtime_error);
    CHECK_NOTHROW(baron.gather());
}

TEST_CASE("Coin requirements for actions") {
    Game game;
    Governor governor(game);
    Spy spy(game);
    Baron baron(game);
    General general(game);
    Judge judge(game);
    Merchant merchant(game);
    std::vector<Player*> players = game.getPlayers();
    // Give everyone 1 coin
    gather_round(players);
    // Try bribe (needs 4 coins)
    CHECK_THROWS_AS(governor.bribe(), std::runtime_error);
    // Try sanction (needs 3 coins)
    CHECK_THROWS_AS(governor.sanction(spy), std::runtime_error);
    // Try coup (needs 7 coins)
    CHECK_THROWS_AS(governor.coup(spy), std::runtime_error);
}

TEST_CASE("Player with 10+ coins must coup") {
    Game game;
    Governor governor(game);
    Spy spy(game);
    Baron baron(game);
    General general(game);
    Judge judge(game);
    std::vector<Player*> players = game.getPlayers();
    // Give governor 10 coins in turn order
    for (int i = 0; i < 10; ++i) gather_round(players);
    CHECK(governor.getcoins() >= 10);
    // All actions except coup should throw
    CHECK_THROWS_AS(governor.gather(), std::runtime_error);
    CHECK_THROWS_AS(governor.tax(), std::runtime_error);
    CHECK_THROWS_AS(governor.bribe(), std::runtime_error);
    CHECK_THROWS_AS(governor.arrest(spy), std::runtime_error);
    CHECK_THROWS_AS(governor.sanction(spy), std::runtime_error);
    // Coup should not throw (if target is valid)
    CHECK_NOTHROW(governor.coup(spy));
}

TEST_CASE("Common player actions in turn order") {
    Game game;
    Governor governor(game);
    Spy spy(game);
    Baron baron(game);
    // General general(game);
    // Judge judge(game);
    // Merchant merchant(game);
    std::vector<Player*> players = game.getPlayers();
    // Gather round 1
    gather_round(players);
    // Gather round 2
    gather_round(players);
    gather_round(players);
    CHECK_NOTHROW(governor.tax());
    // Gather for others
    for (size_t i = 1; i < players.size(); ++i) players[i]->gather();
    for (int i = 0; i < 2; ++i) gather_round(players);
    //governor sanction baron
    CHECK_NOTHROW(governor.sanction(baron));
    // spy arrests governor
    CHECK_NOTHROW(spy.arrest(governor));
    // Sanction 
    CHECK_THROWS_AS(baron.gather(), std::runtime_error);
    CHECK_NOTHROW(baron.bribe());
    CHECK_NOTHROW(baron.tax());
    CHECK_NOTHROW(baron.tax());
    //Coup spy coup baron
    for (int i = 0; i < 3; ++i) gather_round(players);
    CHECK_NOTHROW(governor.tax());
    CHECK_NOTHROW(spy.coup(baron));
}

TEST_CASE("General: cancelcoup ability") {
    Game game;
    Governor governor(game);
    Spy spy(game);
    Baron baron(game);
    General general(game);
    std::vector<Player*> players = game.getPlayers();
    // Give governor enough coins
    for (int i = 0; i < 10; ++i) gather_round(players);
    // General coups Baron
    CHECK_NOTHROW(governor.coup(baron));
    // Now General can cancelcoup on Baron
    CHECK(baron.geteliminated());
    CHECK_NOTHROW(general.cancelcoup(baron));
    CHECK(!baron.geteliminated());
    // cant cancelcoup if the player is not eliminated
    CHECK_THROWS_AS(general.cancelcoup(baron), std::runtime_error);
}

TEST_CASE("Spy: blockArrest and showCoins") {
    Game game;
    Spy spy(game);
    General general(game);
    std::vector<Player*> players = game.getPlayers();
    // Give players enough coins
    gather_round(players); // governor
    gather_round(players); // spy
    gather_round(players); // baron
    // spy taxes and blocks arrest general
    CHECK_NOTHROW(spy.tax());
    CHECK_NOTHROW(spy.blockArrest(general));
    // General tries to arrest spy, but should be blocked
    CHECK_THROWS_AS(general.arrest(spy), std::runtime_error);
    CHECK_NOTHROW(general.tax());
    // showCoins
    int coins = spy.showCoins(general);
    CHECK(coins == general.getcoins());
}

TEST_CASE("Judge: cancelBribe ability") {
    Game game;
    Governor governor(game);
    Spy spy(game);
    Judge judge(game);
    std::vector<Player*> players = game.getPlayers();
    // Give players enough coins
    for (int i = 0; i < 4; ++i) gather_round(players);
    CHECK_NOTHROW(governor.bribe());
    CHECK_NOTHROW(governor.tax());
    CHECK_NOTHROW(governor.tax());
    // Judge cancels bribe
    CHECK_NOTHROW(spy.bribe());
    CHECK_NOTHROW(judge.cancelBribe());
    CHECK_NOTHROW(spy.tax());
    CHECK_THROWS_AS(spy.tax(), std::runtime_error);
    CHECK_THROWS_AS(judge.cancelBribe(), std::runtime_error);
}

TEST_CASE("Governor: cancelTax ability and tax") {
    Game game;
    Spy spy(game);
    Baron baron(game);
    Governor governor(game);
    std::vector<Player*> players = game.getPlayers();
    // Give Baron enough coins for tax
    for (int i = 0; i < 2; ++i) gather_round(players);
    CHECK_NOTHROW(spy.tax());
    CHECK(4 == spy.getcoins());
    // Governor cancels tax
    CHECK_NOTHROW(governor.cancelTax(spy));
    CHECK(2 == spy.getcoins());
    // governor tries to cancel tax again but should throw
    CHECK_NOTHROW(baron.gather());
    CHECK_THROWS_AS(governor.cancelTax(baron), std::runtime_error);
    //tax governor get 3 coins
    int coins = governor.getcoins();
    CHECK_NOTHROW(governor.tax());
    CHECK(coins +3 == governor.getcoins());
}

TEST_CASE("Baron: invest ability") {
    Game game;
    Baron baron(game);
    General general(game);
    Judge judge(game);
     // Not enough coins for invest
    CHECK_THROWS_AS(baron.invest(), std::runtime_error);
    std::vector<Player*> players = game.getPlayers();
    // Give Baron enough coins for invest
    for (int i = 0; i < 3; ++i) gather_round(players);
    int coins = baron.getcoins();
    CHECK_NOTHROW(baron.invest());
    CHECK(coins +3 == baron.getcoins());
    // sanction on baron
    CHECK_NOTHROW(general.sanction(baron));
    CHECK(coins +4 == baron.getcoins());
}

TEST_CASE("Merchant: applyBonus passive ability ") {
    Game game;
    Merchant merchant(game);
    Judge judge(game);
    std::vector<Player*> players = game.getPlayers();
    // Give Merchant 3 coins
    for (int i = 0; i < 3; ++i) gather_round(players);
    // Merchant should get bonus coins
    int before = merchant.getcoins();
    // Next action triggers bonus
    CHECK_NOTHROW(merchant.gather());
    int after = merchant.getcoins();
    CHECK(after == before + 2); // +1 from gather, +1 from bonus

}

TEST_CASE("Game class: add/remove/eliminate/turn/winner") {
    Game game;
    Governor governor(game);
    Baron baron(game);
    Spy spy(game);
    General general(game);
    std::vector<Player*> players = game.getPlayers();
    // eliminatePlayer
    for (int i = 0; i < 10; ++i) gather_round(players);
    CHECK(!game.isEliminated(&baron));
    governor.coup(baron);
    CHECK(game.isEliminated(&baron));
    // removefromeliminatePlayer
    general.cancelcoup(baron);
    CHECK(!game.isEliminated(&baron));
    // getCurrentPlayer
    Player* current = game.getCurrentPlayer();
    CHECK(current == &spy);
    // getPlayers, getEliminatedPlayers
    CHECK(game.getPlayers().size() == 4);
    // checkwinner
    CHECK_NOTHROW(spy.coup(governor));
    CHECK_NOTHROW(general.tax());
    CHECK_NOTHROW(baron.coup(general));
    CHECK_NOTHROW(for (int i = 0; i < 7; ++i) gather_round(game.getPlayers()));
    CHECK_NOTHROW(spy.coup(baron));
    CHECK(game.getgameover());
    CHECK(game.getwinner() == &spy); // spy should be the last player standing
}
