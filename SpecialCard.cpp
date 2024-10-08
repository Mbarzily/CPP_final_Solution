#include "SpecialCard.hpp"
#include "Player.hpp"
#include "Board.hpp"
#include "Bank.hpp"
#include "Game.hpp"
#include <iostream>

SpecialCard::SpecialCard(const std::string& name, int position)
    : Space(name, position) {}

void SpecialCard::landOn(Player* player) {
    SpecialCard card = getRandomCard();
    std::cout << "Card drawn: " << card.getText() << std::endl;
    card.execute(player);
}

void SpecialCard::execute(Player* player) {
    Board& board = Board::getInstance(); // Assuming a singleton Board class

    switch (position) {
        case 1: // Advance to Go (Collect $200)
            player->moveToSpace(0);
            player->addMoney(200);
            break;
        case 2: // Bank pays you dividend of $50
            player->addMoney(50);
            break;
        case 3: // Go back 3 spaces
            player->moveToSpace((player->getPosition() - 3 + 40) % 40);
            Game::getInstance().generateAndDisplayActions();

            break;
        case 4: // Go directly to Jail
            player->goToJail();
            break;
        case 5: // Make general repairs
            {
                int houses = player->getTotalHouseCount();
                int hotels = player->getTotalHotelCount();
                int cost = houses * 25 + hotels * 100;
                player->removeMoney(cost);
                std::cout << player->getName() << " paid $" << cost << " for repairs." << std::endl;
            }
            break;
        case 6: // Pay poor tax of $15
            player->removeMoney(15);
            break;
        case 7: // Take a trip to Reading Railroad
            player->moveToSpace(5);
            if (player->getPosition() > 5) player->addMoney(200);
            break;
        case 8: // Advance to Boardwalk
            player->moveToSpace(39);
            Game::getInstance().generateAndDisplayActions();
            break;
        case 9: // Pay each player $50
            for (auto& otherPlayer : Game::getInstance().getPlayers()) {
                if (otherPlayer.get()->getName() != player->getName()) {
                    Bank::transfer(player,otherPlayer.get(),50);

                    std::cout << player->getName() << " paid for " << otherPlayer->getName() << ": 50$." << std::endl;
                }
            }
            break;
        case 10: // Collect $150
            player->addMoney(150);
            break;
        case 11: // Get out of Jail Free
            std::cout << player->getName() << " received a Get Out of Jail Free card. before: " << player->getGetOutOfJailCardCount();
            player->addGetOutOfJailCard();
            std::cout << "after : " << player->getGetOutOfJailCardCount() << std::endl;
            break;
        case 12: // Advance to Illinois Ave.
            std::cout << player->getName() << " go from: "<< player->getPosition() << "to 24" << std::endl;
            if (player->getPosition() > 24) player->addMoney(200);
            player->moveToSpace(24);
            Game::getInstance().generateAndDisplayActions();
            break;
        case 13: // Advance to St. Charles Place
            std::cout << player->getName() << " go from: "<< player->getPosition() << "to 24" << std::endl;
            if (player->getPosition() > 11) player->addMoney(200);
            player->moveToSpace(11);
            Game::getInstance().generateAndDisplayActions();
            break;
        case 14: // Street repairs
            {
                int houses = player->getTotalHouseCount();
                int hotels = player->getTotalHotelCount();
                std::cout << player->getName() << " has" << houses << " and " << hotels << std::endl;

                int cost = houses * 40 + hotels * 115;
                player->removeMoney(cost);
                std::cout << player->getName() << " paid $" << cost << " for street repairs." << std::endl;
            }
            break;
        case 15: // Advance to nearest Utility
            {
                int currentPos = player->getPosition();
                int newPos = (currentPos > 12 && currentPos < 28) ? 28 : 12;
                // player->moveToSpace(newPos);
                Game::getInstance().buyUtility = true;
                std::pair<int, int> roll = {newPos - currentPos -1, 1};
                Game::getInstance().moveCurrentPlayer(roll);
                Game::getInstance().generateAndDisplayActions();
                std::cout << player->getName() << " arrivre to utility : " << Board::getInstance().getSpace(player->getPosition())->getName() << "from: " << currentPos<< "to: " << player->getPosition() << std::endl;
                

                // Logic for buying or paying rent should be handled in the main game loop
            }
            break;
        case 16: // Advance to nearest Railroad
            {
                int currentPos = player->getPosition();
                int newPos;
                if (currentPos < 5 || currentPos >= 35) newPos = 5;
                else if (currentPos < 15) newPos = 15;
                else if (currentPos < 25) newPos = 25;
                else newPos = 35;
                Game::getInstance().buyUtility = true;
                std::pair<int, int> roll = {newPos - currentPos -1, 1};
                Game::getInstance().moveCurrentPlayer(roll);
                Game::getInstance().generateAndDisplayActions();
                std::cout << player->getName() << " arrivre to utility : " << Board::getInstance().getSpace(player->getPosition())->getName() << "from: " << currentPos<< "to: " << player->getPosition() << std::endl;

                

                // Logic for buying or paying rent should be handled in the main game loop
            }
            break;
    }
    std::cout << "Executed card: " << name << std::endl;
}

SpecialCard SpecialCard::getRandomCard() {
    if (allCards.empty()) {
        initializeCards();
    }
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, allCards.size() - 1);
    return allCards[dis(gen)];
}

void SpecialCard::initializeCards() {
    allCards = {
        SpecialCard("Advance to Go (Collect $200)", 1),
        SpecialCard("Bank pays you dividend of $50", 2),
        SpecialCard("Go back 3 spaces", 3),
        SpecialCard("Go directly to Jail: do not pass Go, \ndo not collect $200", 4),
        SpecialCard("Make general repairs on all your property:  \nFor each house pay $25 -  \nFor each hotel $100", 5),
        SpecialCard("Pay poor tax of $15", 6),
        SpecialCard("Take a trip to Reading Railroad:  \nIf you pass Go collect $200", 7),
        SpecialCard("Take a walk on the Boardwalk:  \nAdvance token to Boardwalk", 8),
        SpecialCard("You have been elected Chairman of the Board:  \nPay each player $50", 9),
        SpecialCard("Your building loan matures:  \nCollect $150", 10),
        SpecialCard("Get out of Jail Free:  \nThis card may be kept until needed or traded", 11),
        SpecialCard("Advance to Illinois Ave:  \nIf you pass Go, collect $200", 12),
        SpecialCard("Advance to St. Charles Place:  \nIf you pass Go, collect $200", 13),
        SpecialCard("You are assessed for street repairs:  \n$40 per     house, $115 per hotel", 14),
        SpecialCard("Advance to nearest Utility: \nIf unowned you may buy it from the Bank. \nIf owned, throw dice and pay owner a \ntotal ten times the amount thrown.", 15),
        SpecialCard("Advance to nearest Railroad: \nIf unowned, you may buy it from the Bank. \nIf owned, pay owner twice the rental \nto which they are otherwise entitled.", 16)
    };
}
std::vector<SpecialCard> SpecialCard::allCards;
