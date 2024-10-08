#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class Property; // Forward declaration
class Street; // Forward declaration


class Property;

class Player {
public:
    Player(const std::string& name,const int id, const sf::Color& color);
    const sf::CircleShape& getToken() const { return token; }

    void setPreviousPosition(int position);
    int getPreviousPosition() const;

    void addMoney(int amount);
    bool removeMoney(int amount);
    void moveToSpace(int spaceIndex);
    void addProperty(Property* property);
    void bankrupt(Player* creditor);
    bool isBankrupt() const;
    std::string getName() const;
    int getPosition() const;
    void goToJail();
    int getMoney() const;
    int getId() const;
    sf::Color getColor() ;
    std::vector<Property*> getProperties() const;

    int getTotalHouseCount() const;
    int getTotalHotelCount() const;

    void setInJail(bool inJail);
    bool isInJail() const;
    bool useGetOutOfJailCard();
    void addGetOutOfJailCard();
    int getGetOutOfJailCardCount() const {return getOutOfJailCards;};
    void getOutOfJail() {if(getOutOfJailCards > 0){inJail = false;getOutOfJailCards--;}};
    bool hasGetOutOfJailCard(){return (getOutOfJailCards > 0);}
    void addDelayTurn(){delayTurn = true;}
    void cancelDelayTurn(){delayTurn = false;}
    bool isDelayTurn(){return delayTurn ;}
    void setMoney(int amount);


    // GUI-related methods
    void draw(sf::RenderWindow& window, const sf::Vector2f& position) const;

private:
    std::string name;
    int id;
    int money;
    int position;
    int previousPosition;
    bool delayTurn = false;
    std::vector<Property*> properties;
    std::vector<Street*> streets;

    bool isBankrupt_;

    bool inJail;
    int getOutOfJailCards;
    
    // GUI-related members
    sf::CircleShape token;
    static constexpr float TOKEN_RADIUS = 10.0f;
};