#include "Board.hpp"
#include "Property.hpp"
#include "BasicSpace.hpp"
#include "Street.hpp"
#include "Game.hpp"
#include "SpecialCard.hpp"
#include "Utility.hpp"
#include <stdexcept>
#include <iostream>
#include <vector>

Board& Board::getInstance() {
    static Board instance;
    return instance;
}


Board::Board() {
    jailPosition = 10; 
    goToJailPosition = 30;

    // initializeBoard();
    if (!boardTexture.loadFromFile(board_jpg)) {
        throw std::runtime_error("Failed to load board texture");
    }
    if (!houseTexture.loadFromFile(house_png)) {
        throw std::runtime_error("Failed to load house texture");
    }
    boardSprite.setTexture(boardTexture);
    boardSprite.setScale(
        BOARD_SIZE / boardSprite.getLocalBounds().width,
        BOARD_SIZE / boardSprite.getLocalBounds().height
    );
    spaces.push_back(std::make_unique<BasicSpace>("GO", 0));
    spaces.push_back(std::make_unique<Street>("Mediterranean Avenue", 1, 60, 2, 50, "Brown"));
    spaces.push_back(std::make_unique<SpecialCard>("Community Chest", 2));
    spaces.push_back(std::make_unique<Street>("Baltic Avenue", 3, 60, 4, 50, "Brown"));
    spaces.push_back(std::make_unique<BasicSpace>("Income Tax", 4));
    spaces.push_back(std::make_unique<Utility>("Reading Railroad", 5, 200));
    spaces.push_back(std::make_unique<Street>("Oriental Avenue", 6, 100, 6, 50, "Light Blue"));
    spaces.push_back(std::make_unique<SpecialCard>("Chance", 7));
    spaces.push_back(std::make_unique<Street>("Vermont Avenue", 8, 100, 6, 50, "Light Blue"));
    spaces.push_back(std::make_unique<Street>("Connecticut Avenue", 9, 120, 8, 50, "Light Blue"));
    spaces.push_back(std::make_unique<Jail>("Jail", 10));
    spaces.push_back(std::make_unique<Street>("St. Charles Place", 11, 140, 10, 100, "Pink"));
    spaces.push_back(std::make_unique<Utility>("Electric Company", 12, 150));
    spaces.push_back(std::make_unique<Street>("States Avenue", 13, 140, 10, 100, "Pink"));
    spaces.push_back(std::make_unique<Street>("Virginia Avenue", 14, 160, 12, 100, "Pink"));
    spaces.push_back(std::make_unique<Utility>("Pennsylvania Railroad", 15, 200));
    spaces.push_back(std::make_unique<Street>("St. James Place", 16, 180, 14, 100, "Orange"));
    spaces.push_back(std::make_unique<SpecialCard>("Community Chest", 17));
    spaces.push_back(std::make_unique<Street>("Tennessee Avenue", 18, 180, 14, 100, "Orange"));
    spaces.push_back(std::make_unique<Street>("New York Avenue", 19, 200, 16, 100, "Orange"));
    spaces.push_back(std::make_unique<BasicSpace>("Free Parking", 20));
    spaces.push_back(std::make_unique<Street>("Kentucky Avenue", 21, 220, 18, 150, "Red"));
    spaces.push_back(std::make_unique<SpecialCard>("Chance", 22));
    spaces.push_back(std::make_unique<Street>("Indiana Avenue", 23, 220, 18, 150, "Red"));
    spaces.push_back(std::make_unique<Street>("Illinois Avenue", 24, 240, 20, 150, "Red"));
    spaces.push_back(std::make_unique<Utility>("B. & O. Railroad", 25, 200));
    spaces.push_back(std::make_unique<Street>("Atlantic Avenue", 26, 260, 22, 150, "Yellow"));
    spaces.push_back(std::make_unique<Street>("Ventnor Avenue", 27, 260, 22, 150, "Yellow"));
    spaces.push_back(std::make_unique<Utility>("Water Works", 28, 150));
    spaces.push_back(std::make_unique<Street>("Marvin Gardens", 29, 280, 24, 150, "Yellow"));
    spaces.push_back(std::make_unique<BasicSpace>("Go To Jail", 30));
    spaces.push_back(std::make_unique<Street>("Pacific Avenue", 31, 300, 26, 200, "Green"));
    spaces.push_back(std::make_unique<Street>("North Carolina Avenue", 32, 300, 26, 200, "Green"));
    spaces.push_back(std::make_unique<SpecialCard>("Community Chest", 33));
    spaces.push_back(std::make_unique<Street>("Pennsylvania Avenue", 34, 320, 28, 200, "Green"));
    spaces.push_back(std::make_unique<Utility> ("Short Line", 35, 200));
    spaces.push_back(std::make_unique<SpecialCard>("Chance", 36));
    spaces.push_back(std::make_unique<Street>("Park Place", 37, 350, 35, 200, "Blue"));
    spaces.push_back(std::make_unique<BasicSpace>("Luxury Tax", 38));
    spaces.push_back(std::make_unique<Street>("Boardwalk", 39, 400, 50, 200, "Blue"));

    std::cout << "Board initialized with " << spaces.size() << " spaces." << std::endl;
}


void Board::addHouse(int propertyIndex) {
    sf::Vector2f position = Game::getInstance().getSpacePosition(propertyIndex);
    sf::Sprite houseSprite;
    houseSprite.setTexture(houseTexture);
    houseSprite.setScale(0.15f, 0.15f); // Adjust scale as needed
    houseSprite.setOrigin(houseSprite.getLocalBounds().width / 2, houseSprite.getLocalBounds().height / 2);
    
    // Add a small offset to prevent complete overlap with player token
    position.x += 10; // Adjust these values as needed
    position.y += 10;
    
    houseSprite.setPosition(position);
    houseSprites.push_back(houseSprite);
}



void Board::draw(sf::RenderWindow& window) {
    
    // Draw properties
    for (const auto& space : spaces) {
        if (auto property = dynamic_cast<Property*>(space.get())) {
            sf::Vector2f position = getSpacePosition(property->getPosition());
            property->draw(window, position);
        }
    }
    
    // Draw houses
    for (const auto& houseSprite : houseSprites) {
        window.draw(houseSprite);
    }
    
    // Draw players
    for (const auto& player : players) {
        sf::Vector2f position = getSpacePosition(player->getPosition());
        player->draw(window, position);
    }
}



Space* Board::getSpace(int position) {
    if (position < 0 || position >= spaces.size()) {
        std::cerr << "Error: Invalid space position " << position << std::endl;
        return nullptr;
    }
    return spaces[position].get();
}



void Board::movePlayer(Player* player, int spaces) {
    int newPosition = (player->getPosition() + spaces) % getSpaceCount();
    player->moveToSpace(newPosition);
}

const std::vector<std::unique_ptr<Player>>& Board::getPlayers() const {
    return players;
}


int Board::getSpaceCount() const {
    return spaces.size();
}   


void Board::handleBankruptcy(Player* player) {
    std::cout << player->getName() << " has gone bankrupt!" << std::endl;
    // Remove player from the game, redistribute properties, etc.
}
void Board::sendPlayerToJail(Player* player) {
    player->moveToSpace(jailPosition);
    Jail* jail = getJail();
    if (jail) {
        jail->addPrisoner(player);
    } else {
        std::cerr << "Error: Could not send player to jail." << std::endl;
    }
}

Jail* Board::getJail() {
    auto jailSpace = dynamic_cast<Jail*>(spaces[jailPosition].get());
    if (!jailSpace) {
        std::cerr << "Error: Jail space not found at position " << jailPosition << std::endl;
    }
    return jailSpace;
}

void Board::declareWinner(Player* player) {
    std::cout << player->getName() << " has won the game!" << std::endl;
}


std::vector<Space*> Board::getProperties() const {
    std::vector<Space*> properties;
    for (const auto& space : spaces) {
        if (dynamic_cast<Property*>(space.get()) || dynamic_cast<Street*>(space.get())) {
            properties.push_back(space.get());
        }
    }
    return properties;
}

// sf::Vector2f Board::getSpacePosition(int spaceIndex) const {
//     float spaceSize = BOARD_SIZE / SPACES_PER_SIDE;
//     int side = spaceIndex / SPACES_PER_SIDE;
//     int sideIndex = spaceIndex % SPACES_PER_SIDE;

//     switch (side) {
//         case 0: // Bottom
//             return sf::Vector2f(BOARD_SIZE - spaceSize * sideIndex - spaceSize / 2, BOARD_SIZE - spaceSize / 2);
//         case 1: // Left
//             return sf::Vector2f(spaceSize / 2, BOARD_SIZE - spaceSize * sideIndex - spaceSize / 2);
//         case 2: // Top
//             return sf::Vector2f(spaceSize * sideIndex + spaceSize / 2, spaceSize / 2);
//         case 3: // Right
//             return sf::Vector2f(BOARD_SIZE - spaceSize / 2, spaceSize * sideIndex + spaceSize / 2);
//         default:
//             return sf::Vector2f(0, 0);
//     }
// }

std::vector<Property*> Board::getPropertiesByColorGroup(const std::string& colorGroup) const {
    std::vector<Property*> result;
    for (auto& space : spaces) {
        if (auto street = dynamic_cast<Street*>(space.get())) {
            if (street->getColorGroup() == colorGroup) {
                result.push_back(street);
            }
        }
    }
    return result;
}
sf::Vector2f Board::getSpaceSize() const {
        float spaceSize = BOARD_SIZE / SPACES_PER_SIDE - 40;
        return sf::Vector2f(spaceSize, spaceSize);
    }

    sf::Vector2f Board::getSpacePosition(int spaceIndex) const {
        sf::Vector2f spaceSize = getSpaceSize();
        int side = spaceIndex / SPACES_PER_SIDE;
        int sideIndex = spaceIndex % SPACES_PER_SIDE;

        switch (side) {
            case 0: // Bottom
                return sf::Vector2f(BOARD_SIZE - spaceSize.x * (sideIndex + 1) - 30, BOARD_SIZE - spaceSize.y + 30);
            case 1: // Left
                return sf::Vector2f(0, BOARD_SIZE - spaceSize.y * (sideIndex + 1) - 50);
            case 2: // Top
                return sf::Vector2f(spaceSize.x * sideIndex, 0);
            case 3: // Right
                return sf::Vector2f(BOARD_SIZE - spaceSize.x, spaceSize.y * sideIndex);
            default:
                return sf::Vector2f(0, 0);
        }
    }

    sf::Vector2f Board::getSpaceCenter(int spaceIndex) const {
        sf::Vector2f topLeft = getSpacePosition(spaceIndex);
        sf::Vector2f spaceSize = getSpaceSize();
        return sf::Vector2f(topLeft.x + spaceSize.x /2 , topLeft.y + spaceSize.y /2);
    }
    
    