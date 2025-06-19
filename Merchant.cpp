#include "Merchant.hpp"
#include <stdexcept>
#include "Game.hpp"

#include "Merchant.hpp"

namespace coup {

Merchant::Merchant(Game& game)
    : Player(game) {}

std::string Merchant::role() const {
    return "Merchant";
}

void Merchant::applyBonus() {
    if (getcoins() >= 3) {
        addcoins(1);
    }
}

void Merchant::gather() {
    applyBonus();
    Player::gather();
}

void Merchant::tax() {
    applyBonus();
    Player::tax();
}

void Merchant::bribe() {
    applyBonus();
    Player::bribe();
}

void Merchant::arrest(Player& p) {
    applyBonus();
    Player::arrest(p);
}

void Merchant::sanction(Player& p) {
    applyBonus();
    Player::sanction(p);
}

void Merchant::coup(Player& p) {
    applyBonus();
    Player::coup(p);
}

}
