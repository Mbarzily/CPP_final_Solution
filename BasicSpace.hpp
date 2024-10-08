#pragma once
#include <iostream>
#include "Space.hpp"
#include "Player.hpp"
#include "Bank.hpp"

class BasicSpace : public Space {
public:
    BasicSpace(const std::string& name, int position) : Space(name, position) {}
    void landOn(Player* player) override {
        if(position == 4 || position == 38){Bank::transferToBank(player,100);}// Tax Space
        else if(position == 20){player->addDelayTurn();} // Parking Space
        std::cout << player->getName() << " landed on " << getName() << std::endl;
    }
};