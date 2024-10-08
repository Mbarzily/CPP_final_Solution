#include "Street.hpp"
#include "Player.hpp"
#include "Board.hpp"
#include "Bank.hpp"
#include <iostream>
#include <cmath>

Street::Street(const std::string& name, int position, int price, int baseRent, int housePrice, const std::string& colorGroup)
    : Property(name, position, price, baseRent, housePrice),
      colorGroup(colorGroup), houseCount(0), hotel(false), baseRent(baseRent), housePrice(housePrice) {
    calculateHotelRent();
}
void Street::calculateHotelRent() {
    hotelRent = baseRent * 32; 
}

void Street::landOn(Player* player) {
    if (isOwned() && getOwner() != player) {
        int rentToPay = getRent();
        if (hotel) {
            rentToPay = rentWithHouses[4];
        } else if (houseCount > 0) {
            rentToPay = getRent();
        }
        Bank::transfer(player, getOwner(), rentToPay);
    } else {
        Property::landOn(player);
    }
}

bool Street::canBuild() const {
        if(!getOwner()) return true;
        if (improvementLevel >= 5) return false;

        auto& board = Board::getInstance();
        auto properties = board.getPropertiesByColorGroup(colorGroup);
        
        // Check if player owns all streets in the color group
        bool ownsAll = std::all_of(properties.begin(), properties.end(), 
            [this](const Property* prop) { return prop->getOwner() == this->getOwner(); });
        
        if (!ownsAll) std::cout << "player not owns all streets in the color group" << std::endl;return false;

        // Check if houses are built evenly
        return std::all_of(properties.begin(), properties.end(), 
            [this](const Property* prop) {
                auto street = dynamic_cast<const Street*>(prop);
                std::cout << " houses arenot built evenly" << std::endl;
                return street && street->improvementLevel <= this->improvementLevel;
            });
    }


void Street::buildHouse(Player* player) {
    if (canBuild() && houseCount <= 4) {
            houseCount++;
            Bank::transferToBank(player, getPrice());
            setOwner(player);
            player->addProperty(this);

            onBuildHouse();  // Hook method for subclass-specific actions
        }
}

void Street::buildHotel(Player* player) {
    if (houseCount == 4 && player->removeMoney(housePrice * 5)) {
        hotel = true;
        houseCount = 0;
        player->addProperty(this);
    }
}
int Street::getRent() {
    if (!getOwner()) return 0;

    if (hotel) {
        return hotel;
    } else if (houseCount > 0) {
        
        return baseRent * static_cast<int>(std::pow(2, houseCount));
    } else {
        return baseRent;
    }
}
int Street::getHouseCount() const { return houseCount; }
bool Street::hasHotel() const { return hotel; }
std::string Street::getColorGroup() const { return colorGroup; }