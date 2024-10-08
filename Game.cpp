#include "Game.hpp"
#include "Bank.hpp"
#include <iostream>
#include <cmath>



std::unique_ptr<Game> Game::instance = nullptr;

Game& Game::getInstance(int numPlayers) {
    if (!instance) {
        instance = std::unique_ptr<Game>(new Game(numPlayers));
    }
    return *instance;
}
                           
Game::Game(int numPlayers) 
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Monopoly"),
      numPlayers(numPlayers),
      currentPlayerIndex(0),
      doublesCount(0),
      diceRolled(false),
      actionsNeedDisplay(false),
      board(Board::getInstance())

       {
    if (!font.loadFromFile(arial_ttf)) {
        std::cerr << "Error loading font\n";
    }
    if (!boardTexture.loadFromFile(board_jpg)) {
        std::cerr << "Error loading board texture\n";
    }
    initializePlayers();
    initializeGUI();
}

void Game::initializePlayers() {
    for (int i = 1; i <= this->numPlayers; ++i) {
        players.push_back(std::make_unique<Player>("Player " + std::to_string(i), i, getPlayerColor(i-1)));
        players.back()->moveToSpace(0);  // Start at the "Go" space
        
    }
}

void Game::initializeGUI() {
    // Set up board sprite
    boardSprite.setTexture(boardTexture);
    boardSprite.setPosition(PANEL_PADDING, (WINDOW_HEIGHT - BOARD_SIZE) / 2);
    float scale = static_cast<float>(BOARD_SIZE) / boardTexture.getSize().x;
    boardSprite.setScale(scale, scale);

    // Set up right panel
    rightPanel.setSize(sf::Vector2f(PANEL_WIDTH, WINDOW_HEIGHT - 2 * PANEL_PADDING));
    rightPanel.setFillColor(sf::Color(240, 240, 240)); // Light gray
    rightPanel.setPosition(WINDOW_WIDTH - PANEL_WIDTH - PANEL_PADDING, PANEL_PADDING);
    rightPanel.setOutlineThickness(2);
    rightPanel.setOutlineColor(sf::Color(200, 200, 200));

    // Set up player info texts
    for (int i = 0; i < this->numPlayers; ++i) {
        sf::RectangleShape playerBack;
        playerBack.setSize(sf::Vector2f(PANEL_WIDTH - 2 * PANEL_PADDING, 40));
        playerBack.setFillColor(players[i]->getToken().getFillColor());
        playerBack.setPosition(WINDOW_WIDTH - PANEL_WIDTH, 10 + i * 50);
        playerInfoBacks.push_back(playerBack);

        sf::Text playerText;
        playerText.setFont(font);
        playerText.setCharacterSize(16);
        playerText.setFillColor(sf::Color::Black);
        playerText.setPosition(WINDOW_WIDTH - PANEL_WIDTH + 10, 20 + i * 50);
        playerText.setString(players[i]->getName() + ": $" + std::to_string(players[i]->getMoney()));
        playerInfoTexts.push_back(playerText);
    }

    // Set up current player text
    currentPlayerText.setFont(font);
    currentPlayerText.setCharacterSize(24);
    currentPlayerText.setFillColor(sf::Color::Black);
    currentPlayerText.setPosition(WINDOW_WIDTH - PANEL_WIDTH + 10, 430);
    currentPlayerText.setString("Current Player: " + players[0]->getName());

    // Set up message text
    messageText.setFont(font);
    messageText.setCharacterSize(18);
    messageText.setFillColor(sf::Color::Black);
    messageText.setPosition(WINDOW_WIDTH - PANEL_WIDTH + 10, WINDOW_HEIGHT - 200);
    messageText.setString("Welcome to Monopoly!");

    // Initialize Roll Dice button
    rollButton.setSize(sf::Vector2f(160, 50));
    rollButton.setFillColor(sf::Color(100, 180, 100));
    rollButton.setPosition(WINDOW_WIDTH - PANEL_WIDTH + 10, WINDOW_HEIGHT - 120);
    rollButtonText.setFont(font);
    rollButtonText.setString("Roll Dice");
    rollButtonText.setCharacterSize(20);
    rollButtonText.setFillColor(sf::Color::White);
    rollButtonText.setPosition(WINDOW_WIDTH - PANEL_WIDTH + 45, WINDOW_HEIGHT - 110);

     // Initialize End Turn button
    endTurnButton.setSize(sf::Vector2f(160, 50));
    endTurnButton.setFillColor(sf::Color(180, 100, 100));
    endTurnButton.setPosition(WINDOW_WIDTH - PANEL_WIDTH + 190, WINDOW_HEIGHT - 120);
    endTurnButtonText.setFont(font);
    endTurnButtonText.setString("End Turn");
    endTurnButtonText.setCharacterSize(20);
    endTurnButtonText.setFillColor(sf::Color::White);
    endTurnButtonText.setPosition(WINDOW_WIDTH - PANEL_WIDTH + 230, WINDOW_HEIGHT - 110);
}
sf::Vector2f Game::getPlayerOffset(int playerIndex, int totalPlayers) const {
    const float radius = 15.0f; // Adjust this value to change the "spread" of players
    const float angle = (playerIndex * 2 * M_PI) / totalPlayers;
    return sf::Vector2f(radius * std::cos(angle), radius * std::sin(angle));
}

sf::Vector2f Game::getSpacePosition(int spaceIndex) const {
    const float boardX = PANEL_PADDING;
    const float boardY = (WINDOW_HEIGHT - BOARD_SIZE) / 2;
    const float spaceSize = BOARD_SIZE / 11;  // Assuming 11 spaces per side

    spaceIndex = spaceIndex % 40;  // Ensure the index wraps around the board

    int side = spaceIndex / 10;
    int sideIndex = spaceIndex % 10;

    sf::Vector2f basePosition;

    switch (side) {
        case 0: // Bottom side
            basePosition = sf::Vector2f(boardX + BOARD_SIZE - spaceSize * (sideIndex + 0.5f), boardY + BOARD_SIZE - spaceSize / 2);
            break;
        case 1: // Left side
            basePosition = sf::Vector2f(boardX + spaceSize / 2, boardY + BOARD_SIZE - spaceSize * (sideIndex + 0.5f));
            break;
        case 2: // Top side
            basePosition = sf::Vector2f(boardX + spaceSize * (sideIndex + 0.5f), boardY + spaceSize / 2);
            break;
        case 3: // Right side
            basePosition = sf::Vector2f(boardX + BOARD_SIZE - spaceSize / 2, boardY + spaceSize * (sideIndex + 0.5f));
            break;
    }

    return basePosition;
}


void Game::run() {
    std::cout << "Game run start" << std::endl;
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
    std::cout << "Game run end" << std::endl;
}


void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                if (rollButton.getGlobalBounds().contains(mousePos) && !diceRolled) {
                    std::cout << "Roll button clicked" << std::endl;
                    Player* currentPlayer = players[currentPlayerIndex].get();
                    // check whethre player need to skip turn
                    if (currentPlayer->isDelayTurn()){
                            currentPlayerText.setString(currentPlayer->getName() + "has Delay turn.");
                            shouldEndTurn = true;
                            currentPlayer->cancelDelayTurn();
                            endTurn();
                            
                            
                        }
                    else{
                        rollDice();
                    }
                    
                }
                else if (endTurnButton.getGlobalBounds().contains(mousePos) && diceRolled) {
                    std::cout << "End turn button clicked" << std::endl;
                    endTurn();
                }
                else {
                    handleActionClick(mousePos);
                }
            }
        }
    }
}




void Game::render() {
    window.clear(sf::Color::White);

    // Draw the board
    window.draw(boardSprite);


    // Draw players
    for (int i = 0; i < players.size(); ++i) {
        const auto& player = players[i];
        playerInfoTexts[i].setString(players[i]->getName() + ": $" + std::to_string(players[i]->getMoney()));
        sf::Vector2f basePosition = getSpacePosition(player->getPosition());
        sf::Vector2f offset = getPlayerOffset(i, players.size());
        sf::Vector2f finalPosition = basePosition + offset;
        player->draw(window, finalPosition);
    }

    // Draw GUI elements
    window.draw(rightPanel);
    for (const auto& back : playerInfoBacks) {
        window.draw(back);
    }
    for (const auto& text : playerInfoTexts) {
        window.draw(text);
    }
    window.draw(currentPlayerText);
    window.draw(messageText);
     // Draw action buttons and texts
    for (const auto& button : actionButtons) {
        window.draw(button);
    }
    for (const auto& text : actionTexts) {
        window.draw(text);
    }
    window.draw(rollButton);
    window.draw(rollButtonText);
    window.draw(endTurnButton);
    window.draw(endTurnButtonText);

    board.draw(window);


    // Draw players
    for (int i = 0; i < players.size(); ++i) {
        const auto& player = players[i];
        playerInfoTexts[i].setString(players[i]->getName() + ": $" + std::to_string(players[i]->getMoney()));
        sf::Vector2f basePosition = getSpacePosition(player->getPosition());
        sf::Vector2f offset = getPlayerOffset(i, players.size());
        sf::Vector2f finalPosition = basePosition + offset;
        player->draw(window, finalPosition);
    }

    // Draw dice
    dice.draw(window);
    window.display();
}



void Game::endTurn() {
    buyUtility = false;

    if (diceRolled || shouldEndTurn) {
        diceRolled = false;
        clearActions();

        currentPlayerIndex = (currentPlayerIndex + shouldEndTurn) % numPlayers;
        shouldEndTurn = true;
        currentPlayerText.setString("Current Player: " + players[currentPlayerIndex]->getName());
        messageText.setString("New turn. Roll the dice!");
    }
}


void Game::update() {
    
     if (dice.isAnimating()) {
        dice.update();
    } else if (diceRolled && messageText.getString() == "Rolling...") {
        std::cout << "Dice roll finished" << std::endl;
        std::pair<int, int> roll = dice.getCurrentRoll();
        messageText.setString("Rolled: " + std::to_string(roll.first) + " + " + std::to_string(roll.second));
        moveCurrentPlayer(roll);
    }

    if (actionsNeedDisplay) {
        std::cout << "Generating and displaying actions" << std::endl;
        generateAndDisplayActions();
        actionsNeedDisplay = false;
    }
    
}
int Game::checkForDoubleLimit(std::pair<int, int> roll){
    bool isDoubles = Dice::isDoubles(roll);

    if (isDoubles) {
        doublesCount++;
        messageText.setString(messageText.getString() + " (Doubles!) ");
        if (doublesCount == 3) {
            messageText.setString(messageText.getString() + "Third doubles in a row! Going to jail!");
            return -1;
        } else {
            std::cout << " currentPlayerIndex" << currentPlayerIndex <<  std::endl;
            messageText.setString(messageText.getString() + "You can roll again.");
            return 1;
        }
    } 
    return 0;


}

int Game::moveCurrentPlayer(const std::pair<int, int>& roll) {
    
    Player* currentPlayer = players[currentPlayerIndex].get();
    Jail* jail = board.getJail();
    
    if (jail->isInJail(currentPlayer) && !(Dice::isDoubles(roll))) {
        actionsNeedDisplay = true;  

        return 0; // Player doesn't move if in jail
    }
    
    int double_result = checkForDoubleLimit(roll);

    if (double_result != 0){
        if (double_result == -1) {
            board.sendPlayerToJail(currentPlayer);
            doublesCount = 0;
            return double_result;
        }else  {
            shouldEndTurn = false;
            std::cout << " currentPlayerIndex" << currentPlayerIndex <<  std::endl;
        }
    }
    else{doublesCount = 0;}
    

    int oldPosition = currentPlayer->getPosition();
    int newPosition = (oldPosition + roll.first + roll.second) % TOTAL_SPACES;

    
    
    currentPlayer->moveToSpace(newPosition);
    actionsNeedDisplay = true;
    
    // Check if player landed on Go To Jail
    if (newPosition == board.getGoToJailPosition()) {
        board.sendPlayerToJail(currentPlayer);
        return -1;
    }
    // Check if player passed Go
    if (newPosition < oldPosition) {
        std::cout << " oldPosition: " << oldPosition << " newPosition: " << newPosition <<  std::endl;

        Bank::transferFromBank(currentPlayer, 200);
        std::cout << currentPlayer->getName() << " collected 200 shekels for passing Go." << std::endl;
    }
    return 0;
}

std::vector<PlayerAction> Game::generatePlayerActions(Player* player, int diceRoll, bool isDoubles) {
    return PlayerActionGenerator::generateActions(player, board, diceRoll, isDoubles,
        [this](const std::string& message) { this->showPopup(message); });
}

void Game::generateAndDisplayActions() {
    std::cout << "Generate and display actions start" << std::endl;

    Player* currentPlayer = players[currentPlayerIndex].get();
    Jail* jail = board.getJail();
    std::vector<PlayerAction> actions;

    if (jail && jail->isInJail(currentPlayer)) {
        actions.emplace_back("Pay 50 fine to leave jail", [this, currentPlayer, jail]() {
            jail->payFine(currentPlayer);
            endTurn();
        });

        if (currentPlayer->hasGetOutOfJailCard()) {
            actions.emplace_back("Use Get Out of Jail Free card", [this, currentPlayer, jail]() {
                jail->useGetOutOfJailCard(currentPlayer);
                endTurn();
            });
        }

    } else {
        std::pair<int, int> roll = dice.getCurrentRoll();
        int totalRoll = roll.first + roll.second;
        bool isDoubles = (roll.first == roll.second);
        actions = generatePlayerActions(currentPlayer, totalRoll, isDoubles);
    }

    currentActions = actions;
    displayActions(currentActions);
    std::cout << "Generate and display actions end" << std::endl;
}


sf::Color Game::getPlayerColor(int index) const {
    const sf::Color playerColors[] = {
        sf::Color(255, 179, 186), // Pastel Red
        sf::Color(255, 223, 186), // Pastel Orange
        sf::Color(255, 255, 186), // Pastel Yellow
        sf::Color(186, 255, 201), // Pastel Green
        sf::Color(186, 225, 255), // Pastel Blue
        sf::Color(223, 186, 255), // Pastel Purple
        sf::Color(255, 186, 255), // Pastel Pink
        sf::Color(199, 206, 234)  // Pastel Lavender
    };
    return playerColors[index % this->numPlayers];
}

void Game::rollDice() {
    buyUtility = false;
    if (!dice.isAnimating() && !diceRolled) {
        std::pair<int, int> roll = Dice::roll();
        dice.animate(window, roll);
        diceRolled = true;
        messageText.setString("Rolling...");

        Player* currentPlayer = players[currentPlayerIndex].get();
        Jail* jail = board.getJail();
        if (jail && jail->isInJail(currentPlayer)) {
            jail->tryToLeaveJail(currentPlayer, roll.first, roll.second);
        }
    }
}


void Game::displayActions(const std::vector<PlayerAction>& actions) {
    // clearActions();

    float buttonWidth = PANEL_WIDTH - 2 * PANEL_PADDING;
    float buttonHeight = 50;
    float buttonSpacing = 10;
    float startY = 500; // Adjust this value as needed

    for (size_t i = 0; i < actions.size(); ++i) {
        sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
        button.setPosition(WINDOW_WIDTH - PANEL_WIDTH + PANEL_PADDING, startY + i * (buttonHeight + buttonSpacing));
        button.setFillColor(sf::Color(100, 100, 100));
        actionButtons.push_back(button);

        sf::Text text;
        text.setFont(font);
        text.setString(actions[i].getDescription());
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);
        text.setPosition(button.getPosition().x + 10, button.getPosition().y + 10);
        actionTexts.push_back(text);
    }
}

int Game::getPlayerChoice(int numActions) {
    int choice;
    do {
        std::cout << "Enter your choice (1-" << numActions << "): ";
        std::cin >> choice;
    } while (choice < 1 || choice > numActions);
    return choice - 1; // Convert to 0-based index
}
void Game::clearActions() {
    actionButtons.clear();
    actionTexts.clear();
    currentActions.clear();
}

void Game::handleActionClick(const sf::Vector2f& mousePos) {
    for (size_t i = 0; i < actionButtons.size(); ++i) {
        if (actionButtons[i].getGlobalBounds().contains(mousePos)) {
            currentActions[i].execute();
            clearActions();
            break;
        }
    }
}
void Game::showPopup(const std::string& message) {
    sf::RectangleShape background(sf::Vector2f(400, 200));
    background.setFillColor(sf::Color(200, 200, 200));
    background.setPosition((WINDOW_WIDTH - 400) / 2, (WINDOW_HEIGHT - 200) / 2);

    sf::Text popupText;
    popupText.setFont(font);
    popupText.setString(message);
    popupText.setCharacterSize(18);
    popupText.setFillColor(sf::Color::Black);
    popupText.setPosition(background.getPosition().x + 20, background.getPosition().y + 20);

    sf::RectangleShape okButton(sf::Vector2f(100, 50));
    okButton.setFillColor(sf::Color(100, 100, 100));
    okButton.setPosition(background.getPosition().x + 150, background.getPosition().y + 130);
    
    
    sf::Text okText;
    okText.setFont(font);
    okText.setString("OK");
    okText.setCharacterSize(20);
    okText.setFillColor(sf::Color::White);
    okText.setPosition(okButton.getPosition().x + 35, okButton.getPosition().y + 10);

    bool popupOpen = true;
    while (popupOpen && window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    if (okButton.getGlobalBounds().contains(mousePos)) {
                        popupOpen = false;
                    }
                }
            }
        }

        // window.clear();
        // render(); // Draw the game state behind the popup
        window.setFramerateLimit(30);
        window.draw(background);
        window.draw(popupText);
        window.draw(okButton);
        window.draw(okText);
        window.display();
    }
}
void Game::handlePlayerBankruptcy(Player* bankruptPlayer) {
    // Remove the bankrupt player from the game
    auto it = std::find_if(players.begin(), players.end(),
        [bankruptPlayer](const std::unique_ptr<Player>& p) { return p.get() == bankruptPlayer; });
    
    if (it != players.end()) {
        players.erase(it);
    }

    // If there's only one player left, they win the game
    if (players.size() == 1) {
        Board::getInstance().   declareWinner(players.front().get());
    }
}