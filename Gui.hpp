#pragma once
#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include <vector>

namespace coup {
class Gui {
public:
    Gui();
    int run();

private:
    void drawSidebar(Game *game, const std::vector<Player*> &players,
                      Player *current, int &selectedPlayerIndex, int &hoveredPlayerIndex);
    sf::RenderWindow window;
    sf::Font font;
};
}
