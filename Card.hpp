#pragma once
#include <string>

class Player;

class Card {
public:
    Card(const std::string& text){};
    virtual void execute(Player* player) = 0;
    virtual ~Card() = default;
    
protected:
    std::string text;
};