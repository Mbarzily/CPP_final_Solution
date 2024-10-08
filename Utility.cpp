
// Utility.cpp
#include "Utility.hpp"
#include "Player.hpp"
#include "Board.hpp"
#include "Game.hpp"

Utility::Utility(const std::string& name, int position, int price)
    : Property(name, position, price, BASE_RENT, 0) {
    maxHouses = 1; // Railroads can't have houses
    housePrice = 0;
}



int Utility::getCount(Player* player) {
    int count = 0;
    Board& board = Board::getInstance();
    for (const auto& space : board.getSpaces()) {
        if (auto UtilitySpace = dynamic_cast<Utility*>(space.get())) {
            if (UtilitySpace->getOwner() == player) {
                count++;
            }
        }
    }
    std::cout << "found " << count << " Utility spaces" <<  std::endl;
    return count;
}

int Utility::getRent() {    
    if (!getOwner()) return 0;
        else if(position == 12 || position == 28  ){
            std::pair<int, int> rollResult = Game::getInstance().dice.getCurrentRoll();
            std::cout << "Should rent price " << ((rollResult.first + rollResult.second) * 10) <<  std::endl;
            return (rollResult.first + rollResult.second) * 10; // 25, 50, 100, 200
        }
        else{
        int UtilityCount = getCount(getOwner());
        std::cout << "Should rent price " << (BASE_RENT * UtilityCount) <<  std::endl;
        return BASE_RENT * UtilityCount; // 50, 100, 150, 200
        }
    }

void Utility::buildHouse(Player* player) {
    if (canBuild()) {
        this->setOwner(player);
        player->addProperty(this);
    }
}
bool Utility::canBuild() const {
    if (!getOwner() && Game::getInstance().buyUtility) {
        return true;       
    }
    else{ return false;}
}
