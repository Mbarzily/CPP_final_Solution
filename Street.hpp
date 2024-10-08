// #pragma once
#include "Property.hpp"
#include <array>

class Street : public Property {
public:
    Street(const std::string& name, int position, int price, int baseRent, int housePrice, const std::string& colorGroup);
    void landOn(Player* player) override;
    int getRent() override ;
    bool canBuild() const override;
    void buildHouse(Player* player);
    void buildHotel(Player* player);
    int getHouseCount() const;
    bool hasHotel() const;;
    std::string getColorGroup() const;
    int getHousePrice() const { return housePrice; } 
    void calculateHotelRent();

    

private:
    int housePrice;
    int houseCount;
    int hotelRent;
    bool hotel;
    int baseRent;
    std::string colorGroup;
    std::array<int, 5> rentWithHouses; // Rent for 1-4 houses and hotel


    void onBuildHouse() override {improvementLevel++;}
};
