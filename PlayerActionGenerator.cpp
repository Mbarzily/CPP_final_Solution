#include "PlayerActionGenerator.hpp"
#include "Player.hpp"
#include "Board.hpp"
#include "Property.hpp"
#include "Utility.hpp"
#include "Bank.hpp"
#include "Street.hpp"
#include "BasicSpace.hpp"
#include "SpecialCard.hpp"
#include "Game.hpp"
#include <iostream>
class Board;



std::vector<PlayerAction> PlayerActionGenerator::generateActions(Player* player, Board& board, int diceRoll, bool isDoubles, PopupFunction showPopup) {
    std::vector<PlayerAction> actions;

    Jail* jail = board.getJail();
    if (jail && jail->isInJail(player)) {
        // Don't generate regular actions for a player in jail
        return actions;
    }

    Space* currentSpace = board.getSpace(player->getPosition());
    if (auto property = dynamic_cast<BasicSpace*>(currentSpace)) {
        currentSpace->landOn(player);
    }
    // Handle landing on different types of spaces
    else if (auto property = dynamic_cast<Property*>(currentSpace)) {
        handlePropertyActions(player, property, actions);
    } else if (auto street = dynamic_cast<Street*>(currentSpace)) {
        handlePropertyActions(player, street, actions);
    } else {
        handleSpecialSpaceActions(player, currentSpace, board, actions, showPopup);
    }


    // Check for bankruptcy
    if (player->getMoney() < 0) {
        actions.emplace_back("Declare Bankruptcy", [player, &board]() {
            player->bankrupt(nullptr);
            board.handleBankruptcy(player);
        });
    }

    // Check for victory condition
    if (player->getMoney() >= 4000) {
        actions.emplace_back("Declare Victory", [player, &board]() {
            board.declareWinner(player);
        });
    }

    return actions;
}

void PlayerActionGenerator::handlePropertyActions(Player* player, Property* property, std::vector<PlayerAction>& actions) {
    bool canBuildResult = property->canBuild();
    std::cout << "canBuild() result: " << (canBuildResult ? "true" : "false") << std::endl;
    if (!player || !property) {
        std::cerr << "Error: Null player or property in handlePropertyActions" << std::endl;
        return;
    }
    if (property->canBuild()) {
        actions.emplace_back("Buy: " + property->getName() + " for " + std::to_string(property->getPrice()) + " shekels", [player, property]() {
            if (Bank::canTransfer(player,property->getPrice())) {
                Bank::transferToBank(player, property->getPrice());
                property->setOwner(player);
                player->addProperty(property);
                Board::getInstance().addHouse(property->getPosition());

                std::cout << player->getName() << " bought " << property->getName() << "." << std::endl;
            } else {
                std::cout << player->getName() << " cannot afford to buy " << property->getName() << "." << std::endl;
            }
        });
    } else {if (property->getOwner() && property->getOwner() != player) {
        std::cout << "Property cannot be built on. Checking ownership." << std::endl;
        int rent = property->getRent();
        int result = Bank::transfer(player,property->getOwner(),rent);
        std::cout << "Checking if " << player->getName() << " can paid " << rent << " shekels rto " << property->getOwner()->getName() << "." << std::endl;

        if (result == 0) {
            std::cout << player->getName() << " paid " << rent << " shekels rent to " << property->getOwner()->getName() << "." << std::endl;
        } else {
            std::cout << player->getName() << " cannot afford to pay the full rent! You out!" << std::endl;
            
        }
    }
    }
}


void PlayerActionGenerator::handleSpecialSpaceActions(Player* player, Space* space, Board& board, std::vector<PlayerAction>& actions, PopupFunction showPopup) {
    if (!space) {
        std::cerr << "Error: Space is null in handleSpecialSpaceActions" << std::endl;
        return;
    }

    std::string spaceName = space->getName();
    std::cout << "Handling special space: " << spaceName << std::endl;

    if (spaceName == "Chance" || spaceName == "Community Chest") {
        actions.emplace_back("Draw a " + spaceName + " card", [player, spaceName, showPopup]() {
            SpecialCard card = SpecialCard::getRandomCard();
            std::cout << "Card drawn: " << card.getText() << std::endl;
            
            // Show popup with card description
            showPopup("Card drawn: " + card.getText());
            
            card.execute(player);
        });
    }
}


void PlayerActionGenerator::handleBuildingActions(Player* player, Board& board, std::vector<PlayerAction>& actions) {
    for (const auto& space : board.getSpaces()) {
        if (auto property = dynamic_cast<Property*>(space.get())) {
            if (property->getOwner() == player && property->canBuild()) {
                actions.emplace_back("Build on " + property->getName(), [player, property]() {
                    property->buildHouse(player, property);
                });
            }
        }
    }
}

void PlayerActionGenerator::handleStreetActions(Player* player, Street* street, std::vector<PlayerAction>& actions) {
    if (!street->isOwned()) {
        actions.emplace_back("Buy " + street->getName() + " for " + std::to_string(street->getPrice()) + " shekels", [player, street]() {
            if (Bank::canTransfer(player,street->getPrice())) {
                street->buildHouse(player);
                std::cout << player->getName() << " bought " << street->getName() << "." << std::endl;
            } else {
                std::cout << player->getName() << " cannot afford to buy " << street->getName() << "." << std::endl;
            }
        });
    } else if (street->getOwner() != player) {
        int rent = street->getRent();
        actions.emplace_back("Pay rent of " + std::to_string(rent) + " shekels to " + street->getOwner()->getName(), [player, street, rent]() {
            if (player->removeMoney(rent)) {
                street->getOwner()->addMoney(rent);
                std::cout << player->getName() << " paid " << rent << " shekels rent to " << street->getOwner()->getName() << "." << std::endl;
            } else {
                std::cout << player->getName() << " cannot afford to pay the rent!" << std::endl;
                player->bankrupt(street->getOwner());
            }
        });
    }
}

