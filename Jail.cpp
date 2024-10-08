// Jail.cpp
#include "Jail.hpp"
#include <iostream>

Jail::Jail(const std::string& name, int position) : Space(name, position) {}

void Jail::landOn(Player* player) {
    std::cout << player->getName() << " is visiting the jail." << std::endl;
}

void Jail::addPrisoner(Player* player) {
    prisoners[player] = 0;
    player->setInJail(true);
    std::cout << player->getName() << " has been sent to jail!" << std::endl;
}

void Jail::releasePrisoner(Player* player) {
    prisoners.erase(player);
    player->setInJail(false);
    std::cout << player->getName() << " has been released from jail! " << std::endl;
}

bool Jail::isInJail(Player* player) const {
    return prisoners.find(player) != prisoners.end();
}

void Jail::tryToLeaveJail(Player* player, int diceRoll1, int diceRoll2) {
    if (!isInJail(player)) return;

    prisoners[player]++;
    
    if (diceRoll1 == diceRoll2) {
        std::cout << player->getName() << " rolled doubles and is released from jail!" << std::endl;
        releasePrisoner(player);
    } else if (prisoners[player] >= MAX_JAIL_TURNS) {
        std::cout << player->getName() << " has been in jail for 3 turns and must pay the fine. pay 50$" << std::endl;
        player->removeMoney(50);

        payFine(player);
    } else {

        std::cout << player->getName() << " failed to roll doubles and remains in jail." << std::endl;
    }
}

void Jail::payFine(Player* player) {
    if (player->removeMoney(JAIL_FINE)) {
        std::cout << player->getName() << " paid the " << JAIL_FINE << " fine and is released from jail." << std::endl;
        releasePrisoner(player);
    } else {
        std::cout << player->getName() << " cannot afford the jail fine!" << std::endl;
        // Handle bankruptcy or other consequences
    }
}

void Jail::useGetOutOfJailCard(Player* player) {
    if (player->useGetOutOfJailCard()) {
        std::cout << player->getName() << " used a Get Out of Jail Free card!" << std::endl;
        releasePrisoner(player);
    } else {
        std::cout << player->getName() << " doesn't have a Get Out of Jail Free card!" << std::endl;
    }
}