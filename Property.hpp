// #pragma once
// #include <SFML/Graphics.hpp>
// #include "Space.hpp"
// #include "Player.hpp"

// class Property : public Space {
// public:
//     Property(const std::string& name, int position, int price, int rent, int improvementCost);
//     void landOn(Player* player) override;
//     virtual bool isOwned() const { return owner != nullptr; }
//     void setOwner(Player* newOwner);
//     std::string getName() const override { return Space::getName(); }
//     Player* getOwner() const { return owner; }
//     int getPrice() const { return price; }
//     int getRent() const { return rent; }
//     int getImprovementLevel() const { return improvementLevel; }
//     void improve();
//     void downgrade();
//     sf::Font getFont(){return font;}
//     int getBoardPosition() const { return position; }
//     // int getHouseCount() const;
//     int getHotelCount() const;
//     int getImprovementCost() const;
//     virtual int getHouseCount() const { return 0; }
//     virtual void addHouse() {}
//     virtual bool canAddHouse() const { return false; }

// private:
//     int price;
//     int rent;
//     Player* owner;
//     int improvementLevel;
//     int improvementCost;

//     // GUI-related members
//     sf::RectangleShape propertyRect;
//     sf::Text nameText;
//     sf::Font font;
//     static constexpr float PROPERTY_WIDTH = 60.0f;
//     static constexpr float PROPERTY_HEIGHT = 80.0f;
//     static constexpr float HOUSE_WIDTH = 10.0f;
//     static constexpr float HOUSE_HEIGHT = 10.0f;


// };

#pragma once
#include "Space.hpp"
#include <SFML/Graphics.hpp>

class Player;

class Property : public Space {
public:
    Property(const std::string& name, int position, int price, int rent, int improvementCost);

    virtual bool canBuild() const = 0;
    virtual void landOn(Player* player) override;
    bool isOwned() const override { return owner != nullptr; }
    virtual int getRent() { return rent; } ;
    void buildHouse(Player* player,Property* property);

    void setOwner(Player* newOwner);
    Player* getOwner() const { return owner; }
    int getPrice() const { return price; }
    // int getRent() const { return rent; }

    void improve();
    void downgrade();
    int getHotelCount() const;
    int getImprovementCost() const;
    void addHouse();
    virtual void draw(sf::RenderWindow& window, const sf::Vector2f& position) const;
    // void buildHouse(Player* player);

    static constexpr float PROPERTY_WIDTH = 60.0f;
    static constexpr float PROPERTY_HEIGHT = 80.0f;
    static constexpr float HOUSE_WIDTH = 10.0f;
    static constexpr float HOUSE_HEIGHT = 10.0f;


protected:
    int price;
    int rent;
    Player* owner;
    int improvementLevel;
    int improvementCost;
    sf::RectangleShape propertyRect;
    sf::Text nameText;
    sf::Font font;
    int houseCount;
    int maxHouses;
    int housePrice;
    virtual void onBuildHouse() {} 
};