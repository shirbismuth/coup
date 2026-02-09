// GUI.cpp - כולל הצגת תור שחקן ופעולה לפי תור בלבד
#include <SFML/Graphics.hpp>
#include "../include/Game.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace coup;

void drawSidebar(Game *game, sf::RenderWindow &window, sf::Font &font, const std::vector<Player *> &players, Player *current, int &selectedPlayerIndex, int &hoveredPlayerIndex)
{
    sf::RectangleShape sidebar(sf::Vector2f(250, 700));
    sidebar.setFillColor(sf::Color(205, 183, 158)); // קרם בהיר
    sidebar.setPosition(0, 0);
    window.draw(sidebar);

    int y = 70;               // Start lower to avoid comment line
    int playerBoxHeight = 60; // Increased from 48 to 60 for more space
    int playerBoxGap = 12;
    for (size_t i = 0; i < players.size(); ++i)
    {
        Player *p = players[i];
        int boxY = y;
        // Lower the hover effect by 4px
        if (i == static_cast<size_t>(hoveredPlayerIndex))
        {
            boxY += 4;
        }
        sf::RectangleShape playerBox(sf::Vector2f(230, playerBoxHeight));
        playerBox.setPosition(10, boxY);

        if (game && game->isEliminated(p))
        {
            playerBox.setFillColor(sf::Color(180, 120, 80)); // חום בהיר למודח
        }
        // If hovered, always use hover color, even for current player
        else if (i == static_cast<size_t>(hoveredPlayerIndex))
        {
            playerBox.setFillColor(sf::Color(170, 210, 140)); // ירוק בהיר לממוקד
        }
        else if (p == current)
        {
            playerBox.setFillColor(sf::Color(60, 120, 60)); // ירוק כהה לשחקן בתור
        }
        else if (i == static_cast<size_t>(selectedPlayerIndex))
        {
            playerBox.setFillColor(sf::Color(220, 220, 180)); // קרם בהיר לנבחר
        }
        else
        {
            playerBox.setFillColor(sf::Color(240, 230, 210)); // קרם רגיל
        }

        playerBox.setOutlineThickness(2);
        playerBox.setOutlineColor(sf::Color(200, 200, 200)); // לבן־בהיר
        window.draw(playerBox);

        std::ostringstream ss;
        ss << "Player " << p->getId() << "\nRole: " << p->role() << "\nCoins: " << p->getcoins();
        sf::Text text(ss.str(), font, 14); // Slightly smaller font
        text.setFillColor(p == current ? sf::Color::Yellow : sf::Color::Black);
        text.setPosition(20, boxY + 10); // Move text a bit lower for better fit
        window.draw(text);

        y += playerBoxHeight + playerBoxGap;
    }

    // עמודה לשחקנים שהודחו
    sf::RectangleShape eliminatedSidebar(sf::Vector2f(240, 700));
    eliminatedSidebar.setFillColor(sf::Color(222, 184, 135)); // קרמל בהיר לעמודת מודחים
    eliminatedSidebar.setPosition(860, 0);
    window.draw(eliminatedSidebar);

    sf::Text eliminatedTitle("Eliminated Players", font, 20);
    eliminatedTitle.setFillColor(sf::Color(180, 180, 180));
    eliminatedTitle.setPosition(870, 10);
    window.draw(eliminatedTitle);

    int eliminatedY = 50;
    for (Player *p : game->getEliminatedPlayers())
    {
        sf::RectangleShape playerBox(sf::Vector2f(220, 70));
        playerBox.setPosition(870, eliminatedY);
        playerBox.setFillColor(sf::Color(210, 140, 100)); // חום־קרמל למודח
        playerBox.setOutlineThickness(2);
        playerBox.setOutlineColor(sf::Color(120, 120, 120));
        window.draw(playerBox);

        std::ostringstream ss;
        ss << "Player " << p->getId() << "\nRole: " << p->role();
        // Removed coins display
        sf::Text text(ss.str(), font, 16);
        text.setFillColor(sf::Color::White);
        text.setPosition(880, eliminatedY + 10);
        window.draw(text);

        eliminatedY += 80;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Coup - GUI Gameboard");
    sf::Font font;
    if (!font.loadFromFile("assets/ARIAL.TTF"))
    {
        std::cerr << "Error loading font" << std::endl;
        return -1;
    }

    enum State
    {
        HOME,
        SELECT_PLAYERS,
        SHOW_ROLES,
        GAME_OVER
    } state = HOME;
    int numPlayers = 2;
    bool mouseWasPressed = false;

    Game *game = nullptr;

    sf::Text title("Welcome to Coup", font, 32);
    title.setPosition(300, 80);
    title.setFillColor(sf::Color(255, 215, 0));
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::Text choosePlayers("Choose number of players (2-6) with UP/DOWN, ENTER to confirm", font, 20);
    choosePlayers.setPosition(200, 250);
    choosePlayers.setFillColor(sf::Color::Black);

    sf::Text numText("2", font, 48);
    numText.setPosition(480, 300);
    numText.setFillColor(sf::Color(101, 67, 33));

    sf::Text turnIndicator("", font, 24);
    turnIndicator.setFillColor(sf::Color::Black);
    turnIndicator.setPosition(320, 10);

    // Update the start button design
    sf::Font junicodeFont;
    if (!junicodeFont.loadFromFile("assets/Junicode.ttf"))
    {
        std::cerr << "Error loading Junicode font" << std::endl;
    }

    sf::RectangleShape startButton(sf::Vector2f(400, 100)); // Larger button
    startButton.setFillColor(sf::Color(222, 184, 135));     // Lighter caramel
    startButton.setOutlineThickness(5);
    startButton.setOutlineColor(sf::Color(139, 69, 19)); // Dark caramel
    startButton.setPosition(
        (window.getSize().x - startButton.getSize().x) / 2,
        (window.getSize().y - startButton.getSize().y) / 2 + 100 // Positioned under the title
    );

    sf::Text startButtonText("START GAME", junicodeFont, 36); // Use Junicode font
    startButtonText.setFillColor(sf::Color(101, 67, 33));     // Brown color
    startButtonText.setStyle(sf::Text::Bold);
    startButtonText.setPosition(
        startButton.getPosition().x + (startButton.getSize().x - startButtonText.getLocalBounds().width) / 2,
        startButton.getPosition().y + (startButton.getSize().y - startButtonText.getLocalBounds().height) / 2 - 10);

    sf::RectangleShape confirmButton(sf::Vector2f(180, 45)); // קטן יותר
    confirmButton.setPosition(410, 360);                     // מיקום קרוב יותר למה שיציג טוב ליד כפתור ה־START
    confirmButton.setFillColor(sf::Color(222, 184, 135));    // צבע חום עדין יותר
    confirmButton.setOutlineThickness(5);
    confirmButton.setOutlineColor(sf::Color(139, 69, 19)); // Dark caramel

    sf::Text confirmButtonText("CONFIRM", junicodeFont, 28);
    confirmButtonText.setFillColor(sf::Color(101, 67, 33));
    confirmButtonText.setStyle(sf::Text::Bold);
    confirmButtonText.setPosition(
        confirmButton.getPosition().x + (confirmButton.getSize().x - confirmButtonText.getLocalBounds().width) / 2,
        confirmButton.getPosition().y + (confirmButton.getSize().y - confirmButtonText.getLocalBounds().height) / 2 - 6);

    // --- Action Buttons Layout ---
    std::vector<sf::RectangleShape> actionButtons;
    std::vector<sf::Text> actionTexts;
    std::vector<std::string> actionNames = {
        "Gather", "Tax", "Bribe", "Arrest", "Sanction",
        "Coup", "Invest", "Cancel Bribe", "Show Coins", "Cancel Tax", "Block arrest"};
    sf::Color actionColors[] = {
        sf::Color(70, 200, 120), sf::Color(70, 120, 200), sf::Color(200, 200, 70),
        sf::Color(200, 70, 120), sf::Color(120, 70, 200), sf::Color(200, 70, 70),
        sf::Color(100, 200, 100), sf::Color(100, 100, 200),
        sf::Color(200, 200, 200), sf::Color(255, 100, 100)};

    // 2 rows, up to 6 buttons per row, at the bottom of the window
    float actionBtnW = 130, actionBtnH = 30; // Smaller buttons
    float actionBtnGap = 18;
    int buttonsPerRow = 6;
    int numRows = (actionNames.size() + buttonsPerRow - 1) / buttonsPerRow;
    float totalBlockHeight = numRows * actionBtnH + (numRows - 1) * actionBtnGap;
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    float startY = windowHeight - totalBlockHeight - 30;
    float row1Count = std::min((int)actionNames.size(), buttonsPerRow);
    float row2Count = (int)actionNames.size() > buttonsPerRow ? (int)actionNames.size() - buttonsPerRow : 0;
    float row1Width = row1Count * actionBtnW + (row1Count - 1) * actionBtnGap;
    float row2Width = row2Count * actionBtnW + (row2Count - 1) * actionBtnGap;
    float row1StartX = (windowWidth - row1Width) / 2;
    float row2StartX = (windowWidth - row2Width) / 2;
    if (row2Count > 0)
    {
        row2StartX += (row1Width - row2Width) / 2 - 60; // Move row 2 a bit to the left
    }

    for (size_t i = 0; i < actionNames.size(); ++i)
    {
        int row = i / buttonsPerRow;
        int col = i % buttonsPerRow;
        float x = (row == 0 ? row1StartX : row2StartX) + col * (actionBtnW + actionBtnGap);
        float y = startY + row * (actionBtnH + actionBtnGap);

        sf::RectangleShape btn(sf::Vector2f(actionBtnW, actionBtnH));
        btn.setPosition((int)x, (int)y);
        btn.setFillColor(sf::Color(210, 180, 140));
        btn.setOutlineThickness(2); // Thinner frame
        btn.setOutlineColor(sf::Color(139, 69, 19));
        actionButtons.push_back(btn);

        // Dynamically adjust font size to fit button height
        unsigned int fontSize = static_cast<unsigned int>(actionBtnH * 0.65);
        sf::Text txt(actionNames[i], font, fontSize);
        txt.setStyle(sf::Text::Bold);
        sf::FloatRect bb = txt.getLocalBounds();
        txt.setPosition(
            (int)(x + (actionBtnW - bb.width) / 2 - bb.left),
            (int)(y + (actionBtnH - bb.height) / 2 - bb.top));
        txt.setFillColor(sf::Color::Black);
        actionTexts.push_back(txt);
    }

    int selectedPlayerIndex = -1;
    int hoveredPlayerIndex = -1;
    bool selectingTarget = false;
    int actionIndex = -1; // Variable to store the last clicked action index
    Player *selectedGovernor = nullptr;
    Player *selectedSpy = nullptr;
    Player *selectedBlocker = nullptr;

    // תיבת הודעות מעוצבת
    sf::RectangleShape commentBox(sf::Vector2f(1000, 50));
    commentBox.setPosition(0, 0);                   // Move the notes bar to the top of the page
    commentBox.setFillColor(sf::Color(25, 25, 35)); // כהה עמוק
    commentBox.setOutlineThickness(3);
    commentBox.setOutlineColor(sf::Color(180, 150, 50)); // מסגרת זהובה־ממלכתית

    sf::Text commentWindow("", font, 20);
    commentWindow.setFillColor(sf::Color(230, 230, 230)); // לבן עדין
    commentWindow.setPosition(15, 10);
    commentWindow.setString("Welcome to Coup. Messages will appear here.");

    auto setComment = [&](const std::string &message)
    {
        commentWindow.setString(message);
    };

    // Design the login screen
    sf::Text loginTitle("WELCOME TO THE GAME COUP", junicodeFont, 64); // Large, centered letters
    loginTitle.setFillColor(sf::Color(101, 67, 33));                   // Brown color
    loginTitle.setStyle(sf::Text::Bold);
    loginTitle.setPosition(
        (window.getSize().x - loginTitle.getLocalBounds().width) / 2,
        (window.getSize().y - loginTitle.getLocalBounds().height) / 2 - 100 // Centered vertically
    );

    std::string mycontainer;

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

                if (selectingTarget && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && actionIndex != 7 && actionIndex != 8 && actionIndex != 9 && actionIndex != 10)
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
                                setComment("It's Player " + std::to_string(game->getCurrentPlayer()->getId()) + "'s turn.");
                                break;
                            case 4:
                                current->sanction(*target);
                                actionPerformed = "sanction";
                                break;
                            case 5:
                                current->coup(*target);
                                actionPerformed = "coup";
                                // Show cancel coup modal only after coup
                                if (!game->getgameover())
                                {
                                    std::vector<Player *> generals;
                                    for (Player *p : game->getPlayers())
                                    {
                                        if (p->role() == "General" && !p->geteliminated())
                                        {
                                            generals.push_back(p);
                                        }
                                    }
                                    Player *lastEliminated = game->getLastCoup();
                                    if (lastEliminated && lastEliminated->role() == "General")
                                    {
                                        generals.push_back(lastEliminated);
                                    }
                                    if (!generals.empty())
                                    {
                                        bool choosingGeneral = true;
                                        while (choosingGeneral && window.isOpen())
                                        {
                                            sf::RectangleShape modalBox(sf::Vector2f(500, 300));
                                            modalBox.setFillColor(sf::Color(245, 222, 179));
                                            modalBox.setOutlineThickness(4);
                                            modalBox.setOutlineColor(sf::Color(139, 69, 19));
                                            modalBox.setPosition((window.getSize().x - 500) / 2, (window.getSize().y - 300) / 2);
                                            window.draw(modalBox);
                                            sf::Text modalTitle("Cancel Coup?", junicodeFont, 36);
                                            modalTitle.setFillColor(sf::Color(101, 67, 33));
                                            modalTitle.setPosition(modalBox.getPosition().x + 40, modalBox.getPosition().y + 20);
                                            window.draw(modalTitle);
                                            float btnY = modalBox.getPosition().y + 80;
                                            std::vector<sf::RectangleShape> generalBtns;
                                            std::vector<sf::Text> generalTexts;
                                            for (size_t i = 0; i < generals.size(); ++i)
                                            {
                                                sf::RectangleShape btn(sf::Vector2f(420, 40));
                                                btn.setFillColor(sf::Color(144, 238, 144));
                                                btn.setOutlineThickness(2);
                                                btn.setOutlineColor(sf::Color(139, 69, 19));
                                                btn.setPosition(modalBox.getPosition().x + 40, btnY);
                                                generalBtns.push_back(btn);
                                                sf::Text txt("General " + std::to_string(generals[i]->getId()) + " (" + generals[i]->role() + ")", junicodeFont, 24);
                                                txt.setFillColor(sf::Color(101, 67, 33));
                                                txt.setPosition(btn.getPosition().x + 10, btn.getPosition().y + 5);
                                                generalTexts.push_back(txt);
                                                btnY += 50;
                                            }
                                            // Exit button
                                            sf::RectangleShape exitBtn(sf::Vector2f(420, 40));
                                            exitBtn.setFillColor(sf::Color(200, 100, 100));
                                            exitBtn.setOutlineThickness(2);
                                            exitBtn.setOutlineColor(sf::Color(139, 69, 19));
                                            exitBtn.setPosition(modalBox.getPosition().x + 40, btnY + 10);
                                            sf::Text exitText("Exit (No General will cancel coup)", junicodeFont, 24);
                                            exitText.setFillColor(sf::Color(101, 67, 33));
                                            exitText.setPosition(exitBtn.getPosition().x + 10, exitBtn.getPosition().y + 5);
                                            window.clear();
                                            // Redraw main game screen here if needed
                                            // ...existing code to redraw main game state...
                                            window.draw(modalBox);
                                            window.draw(modalTitle);
                                            for (size_t i = 0; i < generalBtns.size(); ++i)
                                            {
                                                window.draw(generalBtns[i]);
                                                window.draw(generalTexts[i]);
                                            }
                                            window.draw(exitBtn);
                                            window.draw(exitText);
                                            window.display();
                                            sf::Event modalEvent;
                                            while (window.pollEvent(modalEvent))
                                            {
                                                if (modalEvent.type == sf::Event::Closed)
                                                {
                                                    window.close();
                                                    choosingGeneral = false;
                                                }
                                                if (modalEvent.type == sf::Event::MouseButtonPressed && modalEvent.mouseButton.button == sf::Mouse::Left)
                                                {
                                                    for (size_t i = 0; i < generalBtns.size(); ++i)
                                                    {
                                                        if (generalBtns[i].getGlobalBounds().contains(modalEvent.mouseButton.x, modalEvent.mouseButton.y))
                                                        {
                                                            // General i chosen to perform cancel coup
                                                            Player *lastEliminated = game->getLastCoup();
                                                            if (lastEliminated)
                                                            {
                                                                if (auto g = dynamic_cast<General *>(generals[i]))
                                                                {
                                                                    try
                                                                    {
                                                                        g->cancelcoup(*lastEliminated);
                                                                        setComment("General " + std::to_string(g->getId()) + " returned Player " + std::to_string(lastEliminated->getId()) + " to the game.");
                                                                    }
                                                                    catch (const std::exception &e)
                                                                    {
                                                                        setComment("Error: " + std::string(e.what()));
                                                                    }
                                                                }
                                                            }
                                                            choosingGeneral = false;
                                                        }
                                                    }
                                                    if (exitBtn.getGlobalBounds().contains(modalEvent.mouseButton.x, modalEvent.mouseButton.y))
                                                    {
                                                        choosingGeneral = false;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                if (game->getgameover())
                                {
                                    Player *winner = game->getwinner();
                                    state = GAME_OVER;
                                    setComment(""); // Clear comment
                                }
                                break;

                                setComment("Action performed: " + actionPerformed + ". Performed by Player " + std::to_string(current->getId()) + ".");
                            }
                        }
                        catch (const std::exception &e)
                        {
                            setComment("Error: " + std::string(e.what()));
                        }
                        selectingTarget = false;
                        selectedPlayerIndex = -1;
                    }
                }

                if (selectingTarget && actionIndex == 8)
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
                        if (hoveredPlayerIndex < 0)
                        {
                            setComment("Please click on a valid player.");
                        }
                        else if (!selectedSpy)
                        {
                            Player *candidate = game->getPlayers()[hoveredPlayerIndex];
                            if (candidate && candidate->role() == "Spy")
                            {
                                selectedSpy = candidate;
                                std::cout << "[DEBUG] selectedSpy id=" << candidate->getId() << "\n";
                                setComment("Spy " + std::to_string(candidate->getId()) + " selected. Now choose a target.");
                            }
                            else
                            {
                                setComment("Please select a Spy to perform Show Coins.");
                            }
                        }
                        else
                        {
                            Player *target = game->getPlayers()[hoveredPlayerIndex];
                            if (target)
                            {
                                Spy *s = dynamic_cast<Spy *>(selectedSpy);
                                if (s)
                                {
                                    int coins = s->showCoins(*target);
                                    setComment("Spy #" + std::to_string(s->getId()) +
                                               " shows: Player " + std::to_string(target->getId()) +
                                               " has " + std::to_string(coins) + " coins.");
                                }
                                else
                                {
                                    setComment("Error: selectedSpy is not a Spy.");
                                }
                                // איפוס מצב
                                selectedSpy = nullptr;
                                selectingTarget = false;
                                hoveredPlayerIndex = -1;
                            }
                        }
                    }
                }

                // Restrict hover and selection to Judge players for cancelBribe
                if (selectingTarget && actionIndex == 7)
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
                                    // Advance to the next player's turn
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
                if (selectingTarget && actionIndex == 9)
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
                        if (hoveredPlayerIndex < 0)
                        {
                            setComment("Please click on a valid player.");
                            // לא יוצאים מהפונקציה - מחכים לבחירה תקפה
                        }
                        else if (!selectedGovernor)
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

                if (selectingTarget && actionIndex == 11)
                {
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                    {
                        if (hoveredPlayerIndex < 0)
                        {
                            setComment("Please click on a valid player.");
                        }
                        else if (!selectedBlocker)
                        {
                            Player *candidate = game->getPlayers()[hoveredPlayerIndex];
                            if (candidate && candidate->role() == "Spy")
                            {
                                selectedBlocker = candidate;
                                setComment("Spy " + std::to_string(candidate->getId()) +
                                           " selected. Now choose a player to block arrest on.");
                            }
                            else
                            {
                                setComment("Please select a Spy to block Arrest.");
                            }
                        }
                        else
                        {
                            Player *target = game->getPlayers()[hoveredPlayerIndex];
                            if (target)
                            {
                                Spy *s = dynamic_cast<Spy *>(selectedBlocker);
                                if (s)
                                {
                                    s->blockArrest(*target);
                                    setComment("Spy #" + std::to_string(s->getId()) +
                                               " blocked arrest on Player " +
                                               std::to_string(target->getId()) + ".");
                                }
                                else
                                {
                                    setComment("Error: selectedBlocker is not a Spy.");
                                }
                                selectedBlocker = nullptr;
                                selectingTarget = false;
                                hoveredPlayerIndex = -1;
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
                    }
                }
            }
        }

        // Center the instruction text
        sf::Text instructionText("Choose number of players (2-6) with UP/DOWN, ENTER to confirm", junicodeFont, 20);
        instructionText.setFillColor(sf::Color(101, 67, 33)); // Brown color
        instructionText.setPosition(
            (window.getSize().x - instructionText.getLocalBounds().width) / 2 - 20, // Adjusted to the left
            (window.getSize().y - instructionText.getLocalBounds().height) / 2 - 150);
        window.draw(instructionText);

        // Compare button designs
        // Start button: Larger, lighter caramel, bold text
        // Confirm button: Smaller, darker caramel, bold text
        // Both use Junicode font and are centered relative to their respective positions

        // Update colors to shades of caramel brown, cream, and green
        sf::Color backgroundColor(245, 222, 179);  // Cream
        sf::Color buttonColor(210, 180, 140);      // Caramel brown
        sf::Color buttonOutlineColor(139, 69, 19); // Dark caramel
        sf::Color textColor(101, 67, 33);          // Brown
        sf::Color highlightColor(144, 238, 144);   // Light green

        window.clear(backgroundColor);

        for (size_t i = 0; i < actionButtons.size(); ++i)
        {
            actionButtons[i].setFillColor(buttonColor);
            actionButtons[i].setOutlineColor(buttonOutlineColor);
            actionTexts[i].setFillColor(textColor);
        }

        commentBox.setFillColor(sf::Color(222, 184, 135)); // Lighter caramel
        commentBox.setOutlineColor(buttonOutlineColor);
        commentWindow.setFillColor(textColor);

        // Splash screen redesign
        sf::Text title("Welcome to Coup", font, 48);
        title.setFillColor(sf::Color(144, 238, 144)); // Light green
        title.setStyle(sf::Text::Bold | sf::Text::Underlined);

        // Player selection screen redesign
        sf::Text playerSelectionTitle("SELECT PLAYERS", junicodeFont, 55); // Large, centered text
        playerSelectionTitle.setFillColor(sf::Color(101, 67, 33));         // Light green
        playerSelectionTitle.setStyle(sf::Text::Bold);
        playerSelectionTitle.setPosition(
            (window.getSize().x - playerSelectionTitle.getLocalBounds().width) / 2,
            (window.getSize().y - playerSelectionTitle.getLocalBounds().height) / 2 - 100);

        if (state == SELECT_PLAYERS)
        {
            // Draw player selection title and elements
            window.draw(playerSelectionTitle);
            numText.setString(std::to_string(numPlayers));
            window.draw(choosePlayers);
            window.draw(numText);
            window.draw(confirmButton);
            window.draw(confirmButtonText);
        }
        else if (state == SHOW_ROLES && game)
        {
            // Ensure player selection title is not drawn
            Player *current = game->getCurrentPlayer();
            drawSidebar(game, window, font, game->getPlayers(), current, selectedPlayerIndex, hoveredPlayerIndex);

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
                    for (int i = 0; i < actionButtons.size(); i++)
                    {
                        if (actionButtons[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
                        {
                            try
                            {
                                if (i == 3 || i == 4 || i == 5)
                                { // Exclude case 8 (cancelBribe) from requiring a target
                                    selectingTarget = true;
                                    actionIndex = i;
                                    setComment("Select a target player from the list.");
                                }
                                else if (i == 7)
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
                                else if (i == 8)
                                {
                                    // Show Coins: קודם בוחרים Spy, אחר כך יעד
                                    std::vector<Player *> spies;
                                    for (auto p : game->getPlayers())
                                        if (p->role() == "Spy")
                                            spies.push_back(p);

                                    if (spies.empty())
                                    {
                                        setComment("No Spy available to show coins.");
                                    }
                                    else
                                    {
                                        selectingTarget = true;
                                        actionIndex = i;
                                        selectedSpy = nullptr; // נאפס לבחירה
                                        setComment("Select a Spy to perform Show Coins.");
                                    }
                                }
                                else if (i == 9)
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
                                else if (i == 10)
                                {
                                    std::vector<Player *> spies;
                                    for (Player *p : game->getPlayers())
                                        if (p->role() == "Spy")
                                            spies.push_back(p);

                                    if (spies.empty())
                                        setComment("No Spy available to block arrest.");
                                    else
                                    {
                                        selectedBlocker = nullptr;
                                        selectingTarget = true;
                                        actionIndex = i;
                                        setComment("Select a Spy to block Arrest.");
                                    }
                                }
                                else
                                {
                                    switch (i)
                                    {
                                    case 0:
                                        current->gather();
                                        setComment("It's Player " + std::to_string(game->getCurrentPlayer()->getId()) + "'s turn.");
                                        break;
                                    case 1:
                                        current->tax();
                                        setComment("It's Player " + std::to_string(game->getCurrentPlayer()->getId()) + "'s turn.");
                                        break;
                                    case 2:
                                        current->bribe();
                                        setComment("It's Player " + std::to_string(game->getCurrentPlayer()->getId()) + "'s turn.");
                                        break;
                                    case 6:
                                        if (auto b = dynamic_cast<Baron *>(current))
                                            b->invest();
                                        setComment("It's Player " + std::to_string(game->getCurrentPlayer()->getId()) + "'s turn.");

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

            // Hover logic for action buttons
            auto handleHover = [&](sf::Vector2i mousePos)
            {
                for (size_t i = 0; i < actionButtons.size(); ++i)
                {
                    if (actionButtons[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        actionButtons[i].setScale(1.1f, 1.1f); // Slightly enlarge button
                        actionTexts[i].setScale(1.1f, 1.1f);   // Enlarge text uniformly
                    }
                    else
                    {
                        actionButtons[i].setScale(1.0f, 1.0f); // Reset size
                        actionTexts[i].setScale(1.0f, 1.0f);   // Reset text size
                    }
                }
            };

            // Call handleHover on mouse move
            if (event.type == sf::Event::MouseMoved)
            {
                sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
                handleHover(mousePos);
            }
        }

        // Draw the opening screen if state == HOME
        if (state == HOME)
        {
            window.clear(sf::Color(245, 222, 179)); // Cream background
            window.draw(loginTitle);
            window.draw(startButton);
            window.draw(startButtonText);
            window.display();
            continue;
        }

        if (state == GAME_OVER)
        {
            window.clear(sf::Color(245, 222, 179));
            sf::Text overTitle("GAME OVER", junicodeFont, 64);
            overTitle.setFillColor(sf::Color(101, 67, 33));
            overTitle.setStyle(sf::Text::Bold);
            overTitle.setPosition((window.getSize().x - overTitle.getLocalBounds().width) / 2, 100);
            window.draw(overTitle);
            Player *winner = game ? game->getwinner() : nullptr;
            std::string winnerMsg = winner ? ("Winner: Player " + std::to_string(winner->getId()) + " (" + winner->role() + ")") : "No winner found.";
            sf::Text winnerText(winnerMsg, junicodeFont, 36);
            winnerText.setFillColor(sf::Color(101, 67, 33));
            winnerText.setPosition((window.getSize().x - winnerText.getLocalBounds().width) / 2, 220);
            window.draw(winnerText);
            sf::RectangleShape newGameBtn(sf::Vector2f(320, 70));
            newGameBtn.setFillColor(sf::Color(144, 238, 144));
            newGameBtn.setOutlineThickness(4);
            newGameBtn.setOutlineColor(sf::Color(139, 69, 19));
            newGameBtn.setPosition((window.getSize().x - newGameBtn.getSize().x) / 2, 350);
            window.draw(newGameBtn);
            sf::Text newGameText("START NEW GAME", junicodeFont, 32);
            newGameText.setFillColor(sf::Color(101, 67, 33));
            newGameText.setStyle(sf::Text::Bold);
            newGameText.setPosition(newGameBtn.getPosition().x + (newGameBtn.getSize().x - newGameText.getLocalBounds().width) / 2, newGameBtn.getPosition().y + (newGameBtn.getSize().y - newGameText.getLocalBounds().height) / 2 - 6);
            window.draw(newGameText);
            window.display();
            if (event.type == sf::Event::MouseButtonPressed && newGameBtn.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
            {
                delete game;
                game = new Game();
                state = SELECT_PLAYERS;
                numPlayers = 2;
                setComment("Select the number of players.");
                continue;
            }
            continue;
        }

        window.draw(commentBox);
        window.draw(commentWindow);
        window.display();
    }

    delete game;
    return 0;
}
