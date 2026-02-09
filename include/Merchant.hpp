#pragma once
#include "Player.hpp"

namespace coup {

class Merchant : public Player {
public:
    Merchant(Game& game);
    std::string role() const override;

    void gather() override;
    void tax() override;
    void bribe() override;
    void arrest(Player& p) override;
    void sanction(Player& p) override;
    void coup(Player& p) override;

private:
    void applyBonus(); // בונוס פסיבי בתחילת פעולה
};

}

