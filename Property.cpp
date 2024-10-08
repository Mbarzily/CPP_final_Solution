#include "Property.hpp"
#include "Board.hpp"
#include "Bank.hpp"
#include <iostream>
#include "assets.hpp"


Property::Property(const std::string& name, int position, int price, int rent, int improvementCost)
    : Space(name, position), price(price), rent(rent), owner(nullptr), 
      improvementLevel(0), improvementCost(improvementCost) {
    
    propertyRect.setSize(sf::Vector2f(PROPERTY_WIDTH, PROPERTY_HEIGHT));
    propertyRect.setFillColor(sf::Color::White);
    propertyRect.setOutlineColor(sf::Color::Black);
    propertyRect.setOutlineThickness(2);

    if (!font.loadFromFile(arial_ttf)) {
        std::cerr << "Error loading font\n";
    }

    nameText.setFont(font);
    nameText.setString(name);
    nameText.setCharacterSize(10);
    nameText.setFillColor(sf::Color::Black);
}

void Property::landOn(Player* player) {
    if (owner == nullptr) {
        std::cout << player->getName() << " landed on " << getName() << " which is unowned.\n";
        char choice;
        std::cin >> choice;
        if (choice == 'y') {
            if (player->removeMoney(price)) {
                setOwner(player);
                std::cout << player->getName() << " bought " << getName() << ".\n";
            } else {
                std::cout << player->getName() << " doesn't have enough money to buy " << getName() << ".\n";
            }
        }
    } else if (owner != player) {
        std::cout << player->getName() << " landed on " << getName() << " owned by " << owner->getName() << ".\n";
        std::cout << player->getName() << " must pay $" << rent << " in rent.\n";
        if (Bank::canTransfer(player,rent)) {
            Bank::transfer(player,owner,rent);
            std::cout << player->getName() << " paid $" << rent << " to " << owner->getName() << ".\n";
        } else {
            std::cout << player->getName() << " doesn't have enough money to pay the rent and goes bankrupt!\n";
            player->bankrupt(owner);
        }
    } else {
        std::cout << player->getName() << " landed on their own property: " << getName() << ".\n";
    }
}



void Property::setOwner(Player* newOwner) {
    owner = newOwner;
}

void Property::improve() {
    if (improvementLevel < 5) {
        improvementLevel++;
    }
}

void Property::downgrade() {
    if (improvementLevel > 0) {
        improvementLevel--;
    }
}


int Property::getHotelCount() const {
    return (improvementLevel == 5) ? 1 : 0;
}

int Property::getImprovementCost() const {
    return improvementCost;
}

void Property::draw(sf::RenderWindow& window, const sf::Vector2f& position) const {
    sf::RectangleShape propertyRect = this->propertyRect;
    propertyRect.setPosition(position);
    // window.draw(propertyRect);

    sf::Text nameText = this->nameText;
    nameText.setPosition(position.x, position.y + propertyRect.getSize().y / 2);
    // window.draw(nameText);
}
void Property::addHouse() {
    if (owner && improvementLevel < 5) {
        improvementLevel++;
        std::cout << "A house has been added to " << getName() << ". New improvement level: " << improvementLevel << std::endl;
    } else if (!owner) {
        std::cout << "Cannot add a house to " << getName() << ". The property is not owned." << std::endl;
    } else {
        std::cout << "Cannot add more houses to " << getName() << ". Maximum improvement level reached." << std::endl;
    }
}


void Property::buildHouse(Player* player, Property* property) {
    if (canBuild()) {
            Bank::transferToBank(player, getPrice());
            this->setOwner(player);
            player->addProperty(property);
            std::cout << "i am inside buildHouse " << getName() << std::endl;
        }
}
