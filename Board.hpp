#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Space.hpp"
#include "Player.hpp"
#include "Jail.hpp"

const int BOARD_SIZE = 800; // Adjust this value as needed
const int SPACES_PER_SIDE = 11; // Adjust this value as needed

class Board {
public:
    static Board& getInstance();

    // Delete copy constructor and assignment operator
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

    std::vector<std::unique_ptr<Space>> spaces;
    // Public methods
    Space* getSpace(int position);
    void movePlayer(Player* player, int spaces);
    const std::vector<std::unique_ptr<Player>>& getPlayers() const;
    int getSpaceCount() const;
    static const int JAIL_SPACE_INDEX = 10;
    static int getJailSpaceIndex() { return JAIL_SPACE_INDEX; }

    void sendPlayerToJail(Player* player);
    Jail* getJail();
    int jailPosition;
    int goToJailPosition;
    int getGoToJailPosition() const { return goToJailPosition; }


    // GUI-related methods
    void draw(sf::RenderWindow& window);
    sf::Vector2f getSpacePosition(int spaceIndex) const;
    void handleBankruptcy(Player* player);
    void declareWinner(Player* player);
    // void playTurn(Player* player);
    std::vector<Space*> getProperties() const;
    std::vector<Property*> getPropertiesByColorGroup(const std::string& colorGroup) const;
    sf::Vector2f getSpaceCenter(int spaceIndex) const;
    // sf::Vector2f getSpacePosition(int spaceIndex) const ;
    sf::Vector2f getSpaceSize() const;
    void addHouse(int propertyIndex);
    std::vector<std::unique_ptr<Space>> getAllSpaces();
    static constexpr int SPACES_PER_SIDE = 11;
    const std::vector<std::unique_ptr<Space>>& getSpaces() const { return spaces; }




private:
    Board();
    // Jail
    std::unique_ptr<Jail> jail;
    

    std::vector<std::unique_ptr<Player>> players;
    sf::Texture boardTexture;
    sf::Sprite boardSprite;
    std::vector<sf::Sprite> houseSprites;
    sf::Texture houseTexture;

};
