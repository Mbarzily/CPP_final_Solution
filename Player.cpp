#include "Player.hpp"
#include "Property.hpp"
#include "Street.hpp"
#include "Jail.hpp"
#include "Game.hpp"

Player::Player(const std::string& name, int id, const sf::Color& color)
    : name(name), id(id), money(1500), position(0), isBankrupt_(false), inJail(false), getOutOfJailCards(1) {
    token.setRadius(TOKEN_RADIUS);
    token.setFillColor(color);
    token.setOutlineThickness(2.0f);
    token.setOutlineColor(sf::Color::Black);
    token.setOrigin(TOKEN_RADIUS, TOKEN_RADIUS);
    position = 0;
    
}
void Player::setMoney(int amount) {
    money = amount;
}
void Player::addMoney(int amount) {
    money += amount;
}

bool Player::removeMoney(int amount) {
    if (money >= amount) {
        money -= amount;
        return true;
    }
    return false;
}

void Player::moveToSpace(int spaceIndex) {
    position = spaceIndex;
}

void Player::addProperty(Property* property) {
    properties.push_back(property);
}


bool Player::isBankrupt() const {
    return isBankrupt_;
}
void Player::bankrupt(Player* creditor) {
    isBankrupt_ = true;

    if (creditor) {
        // Transfer all money to the creditor
        creditor->addMoney(money);
        money = 0;

        // Transfer all properties to the creditor
        for (auto& property : properties) {
            property->setOwner(creditor);
            creditor->addProperty(property);
        }
        properties.clear();

        // Transfer all Get Out of Jail Free cards to the creditor
        creditor->getOutOfJailCards += this->getOutOfJailCards;
        getOutOfJailCards = 0;
    } else {
        // Bankruptcy due to bank, reset all properties
        for (auto& property : properties) {
            property->setOwner(nullptr);
        }
        properties.clear();
        money = 0;
        getOutOfJailCards = 0;
    }

    // Notify the game that this player is bankrupt
    Game::getInstance().handlePlayerBankruptcy(this);
}

std::string Player::getName() const {
    return name;
}

int Player::getPosition() const {
    return position;
}


sf::Color Player::getColor(){
    return token.getFillColor();
}

void Player::goToJail() {
    inJail = true;
    position = 10; // jail position
}

void Player::setInJail(bool inJail) {
    this->inJail = inJail;
}
bool Player::isInJail() const {
    return inJail;
}
bool Player::useGetOutOfJailCard() {
    if (getOutOfJailCards > 0) {
        Jail* jail = Board::getInstance().getJail();
        jail->releasePrisoner(this);
        getOutOfJailCards--;
        
        return true;
    }
    return false;
}
void Player::addGetOutOfJailCard() {
    getOutOfJailCards++;
}


int Player::getMoney() const {
    return money;
}
int Player::getId() const {
    return id;
}
std::vector<Property*> Player::getProperties() const {
    return properties;
}

void Player::draw(sf::RenderWindow& window, const sf::Vector2f& position) const {
    sf::CircleShape tokenCopy = token;
    tokenCopy.setPosition(position);
    window.draw(tokenCopy);
}
void Player::setPreviousPosition(int position) {
    previousPosition = position;
}

int Player::getPreviousPosition() const {
    return previousPosition;
}
int Player::getTotalHouseCount() const {
    int count = 0;
    for (const auto& property : properties) {
        if (const auto* prop = dynamic_cast<const Property*>(property)) {
            count ++; 
        }
    }
    return count;
}

int Player::getTotalHotelCount() const {
    int count = 0;
    for (const auto& property : properties) {
        if (const auto* street = dynamic_cast<const Street*>(property)) {
            if (street->hasHotel()) {
                count++;
            }
        }
    }
    return count;
}