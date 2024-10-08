#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.hpp"
#include "Player.hpp"
#include "Board.hpp"
#include "Property.hpp"
#include "BasicSpace.hpp"
#include "Street.hpp"
#include "Utility.hpp"
#include "Jail.hpp"
#include "Bank.hpp"
#include "SpecialCard.hpp"

TEST_CASE("Game Initialization") {
    Game& game = Game::getInstance(4);
    
    SUBCASE("Correct number of players") {
        CHECK(game.getPlayers().size() == 4);
    }
    
    SUBCASE("Players start with correct money") {
        for (const auto& player : game.getPlayers()) {
            CHECK(player->getMoney() == 1500);
        }
    }
    
    SUBCASE("Players start at 'Go'") {
        for (const auto& player : game.getPlayers()) {
            CHECK(player->getPosition() == 0);
        }
    }
}

TEST_CASE("Player Movement") {
    Game& game = Game::getInstance(2);
    game.currentPlayerIndex = 1;
    Player* player = game.players[game.currentPlayerIndex].get();
    
    SUBCASE("Basic movement") {
        game.moveCurrentPlayer({3, 4});
        CHECK(player->getPosition() == 7);
    }
    
    SUBCASE("Wrapping around the board") {
        player->moveToSpace(39);
        game.moveCurrentPlayer({1, 2});
        CHECK(player->getPosition() == 2);
    }
}

TEST_CASE("Property Ownership") {
    Game& game = Game::getInstance(2);
    game.currentPlayerIndex = 1;
    Player* player = game.players[game.currentPlayerIndex].get();
    Street* property = dynamic_cast<Street*>(Board::getInstance().getSpace(3));
    
    SUBCASE("Buying property") {
        int initialMoney = player->getMoney();
        property->buildHouse(player);
        CHECK(property->getOwner() == player);
        CHECK(player->getMoney() == initialMoney - property->getPrice());
    }
    
    SUBCASE("Paying rent") {
        Player* player2 = game.getPlayers()[2].get();
        int initialMoney = player2->getMoney();
        property->landOn(player2);
        CHECK(player2->getMoney() == initialMoney - property->getRent());
    }
}

TEST_CASE("Jail Mechanics") {
    Game& game = Game::getInstance(2);
    game.currentPlayerIndex = 1;
    Player* player = game.players[game.currentPlayerIndex].get();
    Jail* jail = Board::getInstance().getJail();
    
    SUBCASE("Sending player to jail") {
        Board::getInstance().sendPlayerToJail(player);
        CHECK(player->getPosition() == jail->getPosition());
        CHECK(jail->isInJail(player));
    }
    
    SUBCASE("Using Get Out of Jail Free card") {
        jail->addPrisoner(player);
        player->addGetOutOfJailCard();
        CHECK(player->useGetOutOfJailCard() == true);
        CHECK_FALSE(jail->isInJail(player));
    }
}

TEST_CASE("Special Cards") {
    Game& game = Game::getInstance(2);
    game.currentPlayerIndex = 1;
    Player* player = game.players[game.currentPlayerIndex].get();
    
    SUBCASE("Advance to Go") {
        SpecialCard card("Advance to Go", 1);
        card.execute(player);
        CHECK(player->getPosition() == 0);
    }
    
    SUBCASE("Get Out of Jail Free") {
        SpecialCard card("Get Out of Jail Free", 11);
        int initialCards = player->getGetOutOfJailCardCount();
        card.execute(player);
        CHECK(player->getGetOutOfJailCardCount() == initialCards + 1);
    }
}

TEST_CASE("Utility Rent Calculation") {
    Game& game = Game::getInstance(2);
    Player* owner = game.getPlayers()[0].get();
    Player* tenant = game.getPlayers()[1].get();
    Utility* utility = dynamic_cast<Utility*>(Board::getInstance().getSpace(12)); // Electric Company
    
    SUBCASE("One utility owned") {

        utility->setOwner(owner);
        std::pair<int, int> roll = Game::getInstance().dice.getCurrentRoll();
        int expectedRent = (roll.first + roll.second )* 10;
        CHECK(utility->getRent() == expectedRent);
    }
    
}

TEST_CASE("Street Improvements") {
    Game& game = Game::getInstance(2);
    game.currentPlayerIndex = 1;
    Player* player = game.players[game.currentPlayerIndex].get();

    Street* street = dynamic_cast<Street*>(Board::getInstance().getSpace(1)); // Mediterranean Avenue
    
    
    SUBCASE("Building houses") {
        street->setOwner(player);
        for (int i = 0; i < 4; ++i) {
            street->buildHouse(player);
            CHECK(street->getHouseCount() == 0);
        }   
        street->buildHouse(player);
        CHECK(street->getHouseCount() == 0); // Can't build 5th house
    }
    
}

TEST_CASE("Bankruptcy and Asset Transfer") {
    Game& game = Game::getInstance(2);
    Player* player = game.players[1].get();
    Player* owner = game.players[2].get();
    
    // Set up initial conditions
    player->setMoney(500);
    owner->setMoney(1000);
    
    // Give player some properties
    Property* prop1 = dynamic_cast<Property*>(Board::getInstance().getSpace(1));
    Property* prop2 = dynamic_cast<Property*>(Board::getInstance().getSpace(3));
    player->addProperty(prop1);
    player->addProperty(prop2);
    prop1->setOwner(player);
    prop2->setOwner(player);
    
    // Give player a Get Out of Jail Free card
    player->addGetOutOfJailCard();
    
    int initialOwnerMoney = owner->getMoney();
    int initialPlayerMoney = player->getMoney();
    int initialOwnerProperties = owner->getProperties().size();
    int initialPlayerProperties = player->getProperties().size();
    int initialOwnerJailCards = owner->getGetOutOfJailCardCount();
    int initialPlayerJailCards = player->getGetOutOfJailCardCount();

    SUBCASE("Player goes bankrupt") {
        // Attempt to transfer more money than the player has
        CHECK(Bank::transfer(player, owner, 600) == 0);
        
        // Check if player is bankrupt
        CHECK(player->isBankrupt());
        
        // Check if all money was transferred
        CHECK(owner->getMoney() == initialOwnerMoney + initialPlayerMoney);
        CHECK(player->getMoney() == 0);
        
        // Check if all properties were transferred
        CHECK(owner->getProperties().size() == initialOwnerProperties + initialPlayerProperties);
        CHECK(player->getProperties().size() == 0);
        CHECK(prop1->getOwner() == owner);
        CHECK(prop2->getOwner() == owner);
        
        // Check if Get Out of Jail Free card was transferred
        CHECK(owner->getGetOutOfJailCardCount() == initialOwnerJailCards + initialPlayerJailCards);
        CHECK(player->getGetOutOfJailCardCount() == 0);
    }
}

TEST_CASE("Doubles Rolling") {
    Game& game = Game::getInstance(2);
    game.currentPlayerIndex = 1;
    Player* player = game.players[game.currentPlayerIndex].get();
    // set dice to double result
    Game::getInstance().dice.setCurrentRoll({3,3});

    
    SUBCASE("Rolling doubles allows another turn") {
        game.moveCurrentPlayer(Game::getInstance().dice.getCurrentRoll());
        CHECK(Game::getInstance().shouldEndTurn == false);
    }
    
    SUBCASE("Three consecutive doubles sends to jail") {
        game.moveCurrentPlayer(Game::getInstance().dice.getCurrentRoll());
        game.moveCurrentPlayer(Game::getInstance().dice.getCurrentRoll());

        CHECK(player->getPosition() == Board::getInstance().jailPosition);
    }
}

TEST_CASE("Passing Go") {
    
    Game& game = Game::getInstance(3);
    game.currentPlayerIndex = 1;
    Player* player = game.players[game.currentPlayerIndex].get();
    
    SUBCASE("Collect $200 for passing Go") {
        int initialMoney = player->getMoney();
        // set the releative position, so the player will run the whole board, and go through "pass".
        Game::getInstance().dice.setCurrentRoll({(40 - player->getPosition() / 2) + 1,(40 - player->getPosition() / 2) + 1});

        game.moveCurrentPlayer(Game::getInstance().dice.getCurrentRoll());

        CHECK(player->getMoney() == initialMoney + 200);
    }
    
    SUBCASE("Don't collect when moving backwards past Go") {
        player->moveToSpace(1);
        int initialMoney = player->getMoney();
        SpecialCard card("Go back 3 spaces", 3);
        card.execute(player);
        CHECK(player->getMoney() == initialMoney);
    }
}

TEST_CASE("Property Groups") {
    Board& board = Board::getInstance();
    
    SUBCASE("Correct number of properties in color groups") {
        CHECK(board.getPropertiesByColorGroup("Brown").size() == 2);
        CHECK(board.getPropertiesByColorGroup("Blue").size() == 2);
        CHECK(board.getPropertiesByColorGroup("Green").size() == 3);
    }
    
    SUBCASE("Properties in correct order") {
        auto orangeProperties = board.getPropertiesByColorGroup("Orange");
        CHECK(orangeProperties[0]->getPosition() < orangeProperties[1]->getPosition());
        CHECK(orangeProperties[1]->getPosition() < orangeProperties[2]->getPosition());
    }
}

TEST_CASE("Bank Transactions") {
    Player player("Test Player", 1, sf::Color::Red);
    
    SUBCASE("Transfer to bank") {
        int initialMoney = player.getMoney();
        Bank::transferToBank(&player, 500);
        CHECK(player.getMoney() == initialMoney - 500);
    }
    
    SUBCASE("Transfer from bank") {
        int initialMoney = player.getMoney();
        Bank::transferFromBank(&player, 300);
        CHECK(player.getMoney() == initialMoney + 300);
    }
}

TEST_CASE("Chance and Community Chest Spaces") {
    Game& game = Game::getInstance(2);
    game.currentPlayerIndex = 1;
    Player* player = game.players[game.currentPlayerIndex].get();
    
    SUBCASE("Landing on Chance space") {
        player->moveToSpace(7); // Chance space
        CHECK_NOTHROW(game.generateAndDisplayActions());
    }
    
    SUBCASE("Landing on Community Chest space") {
        player->moveToSpace(2); // Community Chest space
        CHECK_NOTHROW(game.generateAndDisplayActions());
    }
}

TEST_CASE("Railroad Rent Calculation") {
    Game& game = Game::getInstance(8);
    Player* owner1 = game.getPlayers()[1].get();
    Player* owner2 = game.getPlayers()[2].get();
    Player* owner3 = game.getPlayers()[3].get();
    Player* owner4 = game.getPlayers()[4].get();
    Player* tenant = game.getPlayers()[2].get();
    
    Property* railroad1 = dynamic_cast<Property*>(Board::getInstance().getSpace(5));  // Reading Railroad
    Property* railroad2 = dynamic_cast<Property*>(Board::getInstance().getSpace(15)); // Pennsylvania Railroad
    Property* railroad3 = dynamic_cast<Property*>(Board::getInstance().getSpace(25)); // B. & O. Railroad
    Property* railroad4 = dynamic_cast<Property*>(Board::getInstance().getSpace(35)); // Short Line
    
    SUBCASE("One railroad owned") {
        railroad1->setOwner(owner1);
        CHECK(railroad1->getRent() == 50);
    }
    
    SUBCASE("Two railroads owned") {
        railroad2->setOwner(owner1);
        CHECK(railroad1->getRent() == 100);
        CHECK(railroad2->getRent() == 100);
    }
    
    SUBCASE("Three railroads owned") {
        railroad3->setOwner(owner1);
        CHECK(railroad1->getRent() == 150);
        CHECK(railroad2->getRent() == 150);
        CHECK(railroad3->getRent() == 150);
    }
    
    SUBCASE("Four railroads owned") {
        railroad4->setOwner(owner1);
        CHECK(railroad1->getRent() == 200);
        CHECK(railroad2->getRent() == 200);
        CHECK(railroad3->getRent() == 200);
        CHECK(railroad4->getRent() == 200);
    }
}

TEST_CASE("Income Tax Space") {
    Game& game = Game::getInstance(2);
    game.currentPlayerIndex = 1;
    Player* player = game.players[game.currentPlayerIndex].get();

    int initialMoney = player->getMoney();
    
    BasicSpace* property = dynamic_cast<BasicSpace*>(Board::getInstance().getSpace(4));
    property->landOn(player);
        
    
    CHECK(player->getMoney() == initialMoney - 100);
}

TEST_CASE("Luxury Tax Space") {
    Game& game = Game::getInstance(2);
    game.currentPlayerIndex = 1;
    Player* player = game.players[game.currentPlayerIndex].get();

    int initialMoney = player->getMoney();
    
    BasicSpace* property = dynamic_cast<BasicSpace*>(Board::getInstance().getSpace(38));
    property->landOn(player);
    
    CHECK(player->getMoney() == initialMoney - 100);
}

TEST_CASE("Free Parking") {
    Game& game = Game::getInstance(2);
    game.currentPlayerIndex = 1;
    Player* player = game.players[game.currentPlayerIndex].get();

    int initialMoney = player->getMoney();
    
    player->moveToSpace(20); // Free Parking space
    game.generateAndDisplayActions();
    
    CHECK(player->getMoney() == initialMoney); // Ensure no money is gained or lost
}
