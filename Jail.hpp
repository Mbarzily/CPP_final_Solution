// Jail.hpp
#pragma once
#include "Space.hpp"
#include "Player.hpp"
#include <unordered_map>


class Jail : public Space {
public:
    Jail(const std::string& name, int position);
    virtual void landOn(Player* player) override;
    void addPrisoner(Player* player);
    void releasePrisoner(Player* player);
    bool isInJail(Player* player) const;
    void tryToLeaveJail(Player* player, int diceRoll1, int diceRoll2);
    void payFine(Player* player);
    void useGetOutOfJailCard(Player* player);

private:
    std::unordered_map<Player*, int> prisoners; // Player and number of turns in jail
    const int JAIL_FINE = 50;
    const int MAX_JAIL_TURNS = 3;
};

