// GUI.cpp - כולל הצגת תור שחקן ופעולה לפי תור בלבד
#include <SFML/Graphics.hpp>
#include "Gui.hpp"
#include "Game.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace coup;

namespace coup {
Gui::Gui() : window(sf::VideoMode(1000, 600), "Coup - GUI Gameboard") {
    if(!font.loadFromFile("ARIAL.TTF")) {
        std::cerr << "Error loading font" << std::endl;
    }
}


void Gui::drawSidebar(Game *game, const std::vector<Player*> &players, Player *current, int &selectedPlayerIndex, int &hoveredPlayerIndex)
{
    sf::RectangleShape sidebar(sf::Vector2f(250, 600));
    sidebar.setFillColor(sf::Color(30, 30, 30));
    sidebar.setPosition(0, 0);
    window.draw(sidebar);

    int y = 70; // Adjust starting position to account for the height of the comment box
    for (size_t i = 0; i < players.size(); ++i)
    {
        Player *p = players[i];

        sf::RectangleShape playerBox(sf::Vector2f(230, 70));
        playerBox.setPosition(10, y);
        // Check if the player is eliminated and update the box color accordingly
        if (game && game->isEliminated(p))
        {
            playerBox.setFillColor(sf::Color(100, 100, 100)); // Gray color for eliminated players
            playerBox.setOutlineColor(sf::Color(50, 50, 50)); // Darker outline
        }
        else if (p == current)
        {
            playerBox.setFillColor(sf::Color(50, 50, 50)); // Default color for self
            playerBox.setOutlineColor(sf::Color::Red);     // Optional: Highlight self as unselectable
        }
        else if (i == static_cast<size_t>(hoveredPlayerIndex))
        {
            playerBox.setFillColor(sf::Color(150, 150, 250)); // Hover color
        }
        else if (i == static_cast<size_t>(selectedPlayerIndex))
        {
            playerBox.setFillColor(sf::Color(100, 100, 200)); // Selected color
        }
        else
        {
            playerBox.setFillColor(sf::Color(60, 60, 80)); // Default color
        }
        playerBox.setOutlineThickness(2);
        playerBox.setOutlineColor(sf::Color::White);
        window.draw(playerBox);

        std::ostringstream ss;
        ss << "Player " << p->getId() << "\nRole: " << p->role() << "\nCoins: " << p->coins();
        sf::Text text(ss.str(), font, 16);
        text.setFillColor(p == current ? sf::Color::Red : sf::Color::White);
        text.setPosition(20, y + 10);
        window.draw(text);

        y += 80;
    }

    // Draw the eliminated players on the right side of the screen
    sf::RectangleShape eliminatedSidebar(sf::Vector2f(240, 600)); // Slightly smaller width
    eliminatedSidebar.setFillColor(sf::Color(20, 20, 20));
    eliminatedSidebar.setPosition(760, 0); // Adjusted position to fit within the window
    window.draw(eliminatedSidebar);

    // Title for eliminated players
    sf::Text eliminatedTitle("Eliminated Players", font, 20);
    eliminatedTitle.setFillColor(sf::Color::White);
    eliminatedTitle.setPosition(770, 10); // Adjusted position for better alignment
    window.draw(eliminatedTitle);

    int eliminatedY = 50; // Start below the title
    // Use getEliminatedPlayers instead of isEliminated
    for (Player *p : game->getEliminatedPlayers())
    {
        sf::RectangleShape playerBox(sf::Vector2f(220, 70));
        playerBox.setPosition(770, eliminatedY);          // Position on the right side
        playerBox.setFillColor(sf::Color(100, 100, 100)); // Gray color for eliminated players
        playerBox.setOutlineThickness(2);
        playerBox.setOutlineColor(sf::Color(50, 50, 50));
        window.draw(playerBox);

        std::ostringstream ss;
        ss << "Player " << p->getId() << "\nRole: " << p->role() << "\nCoins: " << p->coins();
        sf::Text text(ss.str(), font, 16);
        text.setFillColor(sf::Color::White);
        text.setPosition(780, eliminatedY + 10);
        window.draw(text);

        eliminatedY += 80; // Move down for the next eliminated player
    }
}

int Gui::run()
{

    enum State
    {
        HOME,
        SELECT_PLAYERS,
        SHOW_ROLES
    } state = HOME;
    int numPlayers = 2;
    bool mouseWasPressed = false;

    Game *game = nullptr;

    sf::Text title("Welcome to Coup", font, 32);
    title.setPosition(300, 80);
    title.setFillColor(sf::Color(255, 215, 0));
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::Text choosePlayers("Choose number of players (2-6) with UP/DOWN, ENTER to confirm", font, 20);
    choosePlayers.setPosition(300, 250);
    choosePlayers.setFillColor(sf::Color::White);

    sf::Text numText("2", font, 48);
    numText.setPosition(470, 300);
    numText.setFillColor(sf::Color::Cyan);

    sf::Text turnIndicator("", font, 24);
    turnIndicator.setFillColor(sf::Color::White);
    turnIndicator.setPosition(320, 10);

    sf::RectangleShape startButton(sf::Vector2f(200, 50));
    startButton.setPosition(400, 200);
    startButton.setFillColor(sf::Color(100, 200, 100));
    sf::Text startButtonText("Start", font, 28);
    startButtonText.setPosition(470, 210);
    startButtonText.setFillColor(sf::Color::Black);

    sf::RectangleShape confirmButton(sf::Vector2f(200, 50));
    confirmButton.setPosition(400, 370);
    confirmButton.setFillColor(sf::Color(70, 120, 200));
    sf::Text confirmButtonText("Confirm", font, 28);
    confirmButtonText.setPosition(455, 380);
    confirmButtonText.setFillColor(sf::Color::Black);

    float actionBtnW = 120, actionBtnH = 40;
    float actionBtnGap = 20;
    float actionStartX = 40;
    float actionStartY = 480;

    std::vector<sf::RectangleShape> actionButtons;
    std::vector<sf::Text> actionTexts;
    std::vector<std::string> actionNames = {
        "Gather", "Tax", "Bribe", "Arrest", "Sanction",
        "Coup", "Invest", "Cancel Coup", "Cancel Bribe", "Show Coins", "Cancel Tax"};
    sf::Color actionColors[] = {
        sf::Color(70, 200, 120), sf::Color(70, 120, 200), sf::Color(200, 200, 70),
        sf::Color(200, 70, 120), sf::Color(120, 70, 200), sf::Color(200, 70, 70),
        sf::Color(100, 200, 100), sf::Color(200, 100, 100), sf::Color(100, 100, 200),
        sf::Color(200, 200, 200), sf::Color(255, 100, 100) // Brighter red for 'Cancel Tax'
    };
    for (size_t i = 0; i < actionNames.size(); ++i)
    {
        int row = (i == 10 ? 0 : i / 5); // Place 'Cancel Tax' in the first row
        int col = (i == 10 ? 5 : i % 5); // Place 'Cancel Tax' next to 'Sanction'
        float x = actionStartX + col * (actionBtnW + actionBtnGap);
        float y = actionStartY + row * (actionBtnH + 10) - (i == 10 ? 15 : 0); // Move 'Cancel Tax' up by 15 pixels
        sf::RectangleShape btn(sf::Vector2f(actionBtnW, actionBtnH));
        btn.setPosition(x, y);
        btn.setFillColor(actionColors[i]);
        btn.setOutlineThickness(2);
        btn.setOutlineColor(sf::Color::Black);
        actionButtons.push_back(btn);
        sf::Text txt(actionNames[i], font, 18);
        txt.setPosition(x + 10, y + 6);
        txt.setFillColor(i == 10 ? sf::Color::White : sf::Color::Black); // White text for 'Cancel Tax'
        actionTexts.push_back(txt);
    }

    int selectedPlayerIndex = -1;
    int hoveredPlayerIndex = -1;
    bool selectingTarget = false;
    int actionIndex = -1; // Variable to store the last clicked action index
    Player *selectedGovernor = nullptr;

    // Adjust the comment box to span the top of the screen
    sf::RectangleShape commentBox(sf::Vector2f(1000, 50)); // Full width of the window
    commentBox.setPosition(0, 0);                          // Top of the screen
    commentBox.setFillColor(sf::Color(30, 30, 30));
    commentBox.setOutlineThickness(2);
    commentBox.setOutlineColor(sf::Color::Yellow);

    sf::Text commentWindow("", font, 20);
    commentWindow.setFillColor(sf::Color::Yellow);
    commentWindow.setPosition(10, 10);                                      // Slight padding from the top-left corner
    commentWindow.setString("Welcome to Coup! Messages will appear here."); // Example message

    auto setComment = [&](const std::string &message)
    {
        commentWindow.setString(message);
    };

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (state == HOME && event.type == sf::Event::MouseButtonPressed && startButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
            {
                game = new Game();
                state = SELECT_PLAYERS;
                setComment("Select the number of players.");
            }
            else if (state == SELECT_PLAYERS)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Up && numPlayers < 6)
                        numPlayers++;
                    else if (event.key.code == sf::Keyboard::Down && numPlayers > 2)
                        numPlayers--;
                }
                else if (event.type == sf::Event::MouseButtonPressed && confirmButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    for (int i = 0; i < numPlayers; ++i)
                        game->createRandomPlayer();
                    state = SHOW_ROLES;
                    setComment("Game started. It's Player " + std::to_string(game->getCurrentPlayer()->getId()) + "'s turn.");
                }
            }
            else if (state == SHOW_ROLES && game)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int y = 20;
                hoveredPlayerIndex = -1;
                for (size_t i = 0; i < game->getPlayers().size(); ++i)
                {
                    sf::FloatRect playerBoxBounds(10, y, 230, 70);
                    // Adjusted hover detection logic to fix the issue where the hover is too high
                    // Updated y-offset for hover detection
                    if (playerBoxBounds.contains(mousePos.x, mousePos.y - 10))
                    { // Adjusted y-offset by -10
                        hoveredPlayerIndex = i;
                        break;
                    }
                    y += 80;
                }

                if (selectingTarget && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && actionIndex != 8 && actionIndex != 10)
                {
                    if (hoveredPlayerIndex >= 0 && hoveredPlayerIndex < game->getPlayers().size())
                    {
                        Player *target = game->getPlayers()[hoveredPlayerIndex];
                        Player *current = game->getCurrentPlayer();
                        try
                        {
                            std::string actionPerformed;
                            switch (actionIndex)
                            {
                            case 3:
                                current->arrest(*target);
                                actionPerformed = "arrest";
                                break;
                            case 4:
                                current->sanction(*target);
                                actionPerformed = "sanction";
                                break;
                            case 5:
                                current->coup(*target);
                                actionPerformed = "coup";
                                break;
                            case 7:
                                if (auto g = dynamic_cast<General *>(current))
                                {
                                    g->cancelcoup(*target);
                                    actionPerformed = "cancel coup";
                                }
                                break;
                            case 9:
                                if (auto s = dynamic_cast<Spy *>(current))
                                {
                                    s->showcoins(*target);
                                    actionPerformed = "show coins";
                                }
                                break;
                            }
                            setComment("Action performed: " + actionPerformed + ". Performed by Player " + std::to_string(current->getId()) + ".");
                        }
                        catch (const std::exception &e)
                        {
                            setComment("Error: " + std::string(e.what()));
                        }
                        selectingTarget = false;
                        selectedPlayerIndex = -1;
                    }
                }

                // Restrict hover and selection to Judge players for cancelBribe
                if (selectingTarget && actionIndex == 8)
                {
                    hoveredPlayerIndex = -1;
                    int y = 20; // Reset y position for accurate hover detection
                    for (size_t i = 0; i < game->getPlayers().size(); ++i)
                    {
                        Player *p = game->getPlayers()[i];
                        if (p->role() == "Judge")
                        {
                            // Adjust bounding box dimensions for better accuracy
                            sf::FloatRect playerBoxBounds(10, y, 230, 70);
                            if (playerBoxBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                            {
                                hoveredPlayerIndex = static_cast<int>(i);
                                break;
                            }
                        }
                        y += 80; // Increment y position for the next player
                    }

                    // Highlight the hovered Judge player
                    if (hoveredPlayerIndex >= 0)
                    {
                        Player *hoveredPlayer = game->getPlayers()[hoveredPlayerIndex];
                        if (hoveredPlayer->role() == "Judge")
                        {
                            setComment("Hovering over Judge Player " + std::to_string(hoveredPlayer->getId()));
                        }
                    }

                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                    {
                        if (hoveredPlayerIndex >= 0)
                        {
                            Player *judge = game->getPlayers()[hoveredPlayerIndex];
                            if (judge->role() == "Judge")
                            {
                                auto j = dynamic_cast<Judge *>(judge);
                                if (j)
                                { // Ensure the cast is successful
                                    j->cancelBribe();
                                    setComment("Player " + std::to_string(judge->getId()) + " (Judge) performed the cancel bribe action.");
                                    selectingTarget = false;
                                    hoveredPlayerIndex = -1;
                                }
                                else
                                {
                                    setComment("Error: Failed to cast Player to Judge.");
                                }
                            }
                            else
                            {
                                setComment("Error: Selected player is not a Judge.");
                            }
                        }
                        else
                        {
                            setComment("Error: No player hovered or selected.");
                        }
                    }
                }

                // Updated logic for cancelTax to ask for both performer and target
                if (selectingTarget && actionIndex == 10)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int y = 20;
                    hoveredPlayerIndex = -1;
                    for (size_t i = 0; i < game->getPlayers().size(); ++i)
                    {
                        Player *p = game->getPlayers()[i];
                        sf::FloatRect box(10, y, 230, 70);
                        if (box.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                        {
                            hoveredPlayerIndex = i;
                            break;
                        }
                        y += 80;
                    }

                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                    {
                        if (!selectedGovernor)
                        {
                            Player *candidate = game->getPlayers()[hoveredPlayerIndex];
                            if (candidate && candidate->role() == "Governor")
                            {
                                selectedGovernor = candidate;
                                setComment("Governor " + std::to_string(candidate->getId()) + " selected. Now choose a target.");
                            }
                            else
                            {
                                setComment("Please select a Governor to perform cancel tax.");
                            }
                        }
                        else
                        {
                            Player *target = game->getPlayers()[hoveredPlayerIndex];
                            if (target)
                            {
                                Governor *g = dynamic_cast<Governor *>(selectedGovernor);
                                if (g)
                                {
                                    g->cancelTax(*target);
                                    setComment("Governor " + std::to_string(selectedGovernor->getId()) + " performed cancel tax on Player " + std::to_string(target->getId()) + ".");
                                }
                                else
                                {
                                    setComment("Error: selected player is not a Governor.");
                                }
                                selectedGovernor = nullptr;
                                selectingTarget = false;
                            }
                        }
                    }
                }

                // Refine hover detection logic for player selection
                if (selectingTarget)
                {
                    hoveredPlayerIndex = -1;
                    int y = 20; // Reset y position for accurate hover detection
                    for (size_t i = 0; i < game->getPlayers().size(); ++i)
                    {
                        Player *p = game->getPlayers()[i];

                        // Adjust bounding box dimensions for better accuracy
                        sf::FloatRect playerBoxBounds(10, y, 230, 70); // x, y, width, height
                        if (playerBoxBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                        {
                            hoveredPlayerIndex = static_cast<int>(i);
                            break;
                        }
                        y += 80; // Increment y position for the next player
                    }

                    // Highlight the hovered player
                    if (hoveredPlayerIndex >= 0)
                    {
                        Player *hoveredPlayer = game->getPlayers()[hoveredPlayerIndex];

                        // Don't overwrite important instruction if in second step of cancel tax
                        if (!(actionIndex == 10 && selectedGovernor))
                        {
                            setComment("Hovering over Player " + std::to_string(hoveredPlayer->getId()) + ", Role: " + hoveredPlayer->role());
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(50, 50, 50));

        if (state == HOME)
        {
            window.draw(title);
            window.draw(startButton);
            window.draw(startButtonText);
        }
        else if (state == SELECT_PLAYERS)
        {
            numText.setString(std::to_string(numPlayers));
            window.draw(choosePlayers);
            window.draw(numText);
            window.draw(confirmButton);
            window.draw(confirmButtonText);
        }
        else if (state == SHOW_ROLES && game)
        {
            Player *current = game->getCurrentPlayer();
            drawSidebar(game, game->getPlayers(), current, selectedPlayerIndex, hoveredPlayerIndex);

            for (size_t i = 0; i < actionButtons.size(); ++i)
            {
                window.draw(actionButtons[i]);
                window.draw(actionTexts[i]);
            }

            // Ensure mouseWasPressed is properly reset and used to handle single clicks
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (!mouseWasPressed)
                {
                    mouseWasPressed = true;
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    for (int i = 0; i < actionButtons.size(); ++i)
                    {
                        if (actionButtons[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
                        {
                            try
                            {
                                if (i == 3 || i == 4 || i == 5 || i == 7 || i == 9)
                                { // Exclude case 8 (cancelBribe) from requiring a target
                                    selectingTarget = true;
                                    actionIndex = i;
                                    setComment("Select a target player from the list.");
                                }
                                else if (i == 8)
                                { // Handle cancelBribe action
                                    std::vector<Player *> judges;
                                    for (Player *p : game->getPlayers())
                                    {
                                        if (p->role() == "Judge")
                                        {
                                            judges.push_back(p);
                                        }
                                    }

                                    if (judges.empty())
                                    {
                                        setComment("No Judge players available to cancel bribes.");
                                    }
                                    else
                                    {
                                        selectingTarget = true;
                                        actionIndex = i;
                                        setComment("Select a Judge player to perform the cancel bribe action.");
                                    }
                                }
                                else if (i == 10)
                                { // Handle cancelTax action
                                    std::vector<Player *> governors;
                                    for (Player *p : game->getPlayers())
                                    {
                                        if (p->role() == "Governor")
                                        {
                                            governors.push_back(p);
                                        }
                                    }

                                    if (governors.empty())
                                    {
                                        setComment("No Governor players available to cancel taxes.");
                                    }
                                    else
                                    {
                                        selectingTarget = true;
                                        actionIndex = i;
                                        setComment("Select a Governor player to perform the cancel tax action.");
                                    }
                                }
                                else
                                {
                                    switch (i)
                                    {
                                    case 0:
                                        current->gather();
                                        break;
                                    case 1:
                                        current->tax();
                                        break;
                                    case 2:
                                        current->bribe();
                                        break;
                                    case 6:
                                        if (auto b = dynamic_cast<Baron *>(current))
                                            b->invest();
                                        break;
                                    }
                                }
                            }
                            catch (const std::exception &e)
                            {
                                setComment("Error: " + std::string(e.what()));
                            }
                        }
                    }
                }
            }
            else
            {
                mouseWasPressed = false;
            }
        }

        window.draw(commentBox);
        window.draw(commentWindow);
        window.display();
    }

    delete game;
    return 0;
}

}
