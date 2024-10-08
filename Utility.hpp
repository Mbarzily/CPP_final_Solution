#pragma once
#include "Property.hpp"
#include <iostream>

class Utility : public Property {
public:
    Utility(const std::string& name, int position,int price);
    int getRent()  override;
    bool canBuild() const override;
    static int getCount(Player* player);
    void buildHouse(Player* player) ;

private:
    static const int PRICE = 200;
    static const int BASE_RENT = 50;
};
