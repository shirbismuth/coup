
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "Game.hpp"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;
using namespace coup;

int main() {
    Game game_1{};

    Governor governor(game_1);
    Spy spy(game_1);
    Baron baron(game_1);
    General general(game_1);
    Judge judge(game_1);

    vector<Player*> players = game_1.getPlayers();
    cout << "players count: " << game_1.getPlayers().size() << endl;
   

    // Expected output: Moshe
    cout << game_1.getCurrentPlayer()->getId() << endl;

    governor.gather();
    spy.gather();
    baron.gather();
    general.gather();
    judge.gather();

    // Expected exception - Not spy's turn
    try{
        spy.gather();
    } catch (const std::exception &e){
        std::cerr << e.what() << '\n';
    }
    cout << "##1" << endl;

    governor.gather();
    cout << "##2" <<  endl;
    spy.tax();
    
    try{
        baron.bribe();
    } catch (const std::exception &e){
        std::cerr << e.what() << '\n';
    }

    return 0;

}

//     // Expected exception - Judge cannot undo tax
//     try{
//         judge.undo(governor);
//     } catch (const std::exception &e) {
//         std::cerr << e.what() << '\n';
//     }

//     cout << governor.coins() << endl; // Expected: 2
//     cout << spy.coins() << endl; // Expected: 3

//     governor.undo(spy); // Governor undo tax
//     cout << spy.coins() << endl; // Expected: 1

//     baron.tax();
//     general.gather();
//     judge.gather(); 

//     governor.tax();
//     spy.gather();
//     baron.invest(); // Baron traded its 3 coins and got 6 
//     general.gather();
//     judge.gather();
    
//     cout << baron.coins() << endl; // Expected: 6

//     governor.tax();
//     spy.gather();
//     baron.gather();
//     general.gather();
//     judge.gather();

//     governor.tax();
//     spy.gather();
//     cout << baron.coins() << endl; // Expected: 7
//     baron.coup(governor); // Coup against governor
//     general.gather();
//     judge.gather();
    
//     players = game_1.players();
//     // Since no one blocked the Baron, the expected output is:
//     // Yossi
//     // Meirav
//     // Reut
//     // Gilad
//     for (string name : players) {
//         cout << name << endl;
//     }

// }
