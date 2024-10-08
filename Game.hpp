#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "assets.hpp"
#include "Player.hpp"
#include "Dice.hpp"
#include "Board.hpp"
#include "PlayerActionGenerator.hpp"
#include "PlayerAction.hpp"

// Temporarily comment out Board include

class Game {
public:
    static Game& getInstance(int numPlayers = 2);

    // Game(int numPlayers);
    void run();
    ~Game() = default;
    int currentPlayerIndex;
    int previousPlayerIndex;
    int numPlayers;
    Board& board;
    std::vector<std::unique_ptr<Player>> players;
    bool buyUtility = false;

    void handlePlayerBankruptcy(Player* bankruptPlayer);

    void showPopup(const std::string& message);
    std::vector<PlayerAction> generatePlayerActions(Player* player, int diceRoll, bool isDoubles);
    const std::vector<std::unique_ptr<Player>>& getPlayers() const {
            return players;
        }
    int moveCurrentPlayer(const std::pair<int, int>& roll);
    void generateAndDisplayActions();
    bool shouldEndTurn = true;

    Dice dice;
    // Placeholder function for getting space position
    sf::Vector2f getSpacePosition(int spaceIndex) const;
    sf::Vector2f getPlayerOffset(int playerIndex, int totalPlayers) const;
    sf::Color getPlayerColor(int index) const;

    
    

private:
    Game(int numPlayers);
    
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    static std::unique_ptr<Game> instance;

    static const int WINDOW_WIDTH = 1280;
    static const int WINDOW_HEIGHT = 960;
    static const int BOARD_SIZE = 830;
    static const int PANEL_WIDTH = 380;
    static const int PANEL_PADDING = 10;
    static const int MAX_PLAYERS = 8;
    static const int TOTAL_SPACES = 40;  // Placeholder for board space count

    sf::RenderWindow window;
    sf::Font font;
    sf::Texture boardTexture;
    sf::Sprite boardSprite;

    // panel

    sf::RectangleShape rightPanel;
    std::vector<sf::RectangleShape> playerInfoBacks;
    std::vector<sf::Text> playerInfoTexts;
    sf::Text currentPlayerText;
    sf::Text messageText;
    sf::RectangleShape endTurnButton;
    sf::Text endTurnButtonText;

    // dice gui

    
    bool diceRolled;
    sf::RectangleShape rollButton;
    sf::Text rollButtonText;
    int doublesCount;
    void rollDice();
    int checkForDoubleLimit(std::pair<int, int> roll);


    // player action
    bool actionsNeedDisplay;
    // bool shouldEndTurn = true;
    std::vector<sf::RectangleShape> actionButtons;
    std::vector<sf::Text> actionTexts;
    std::vector<PlayerAction> currentActions;

    void handleActionClick(const sf::Vector2f& mousePos);

    // void playTurn(Player* player);
    void displayActions(const std::vector<PlayerAction>& actions);
    int getPlayerChoice(int numActions);
    void clearActions();


    // main initial
    void endTurn();
    void initializeGUI();
    void initializePlayers();   
    void processEvents();
    void update();
    void render();

    
    
    


};