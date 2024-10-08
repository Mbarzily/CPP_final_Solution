#pragma once
#include <string>

class Player;  // Forward declaration

class Space {
public:
    Space(const std::string& name, int position) : name(name), position(position) {}
    virtual ~Space() = default;
    virtual std::string getName() const { return name; }
    int getPosition() const { return position; }
    virtual void landOn(Player* player) = 0;  // Pure virtual function
    virtual bool isOwned() const { return false; }  // Default implementation


protected:
    std::string name;
    int position;
};