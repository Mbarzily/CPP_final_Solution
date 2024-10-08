#pragma once
#include <vector>
#include "PlayerAction.hpp"


class Player;
class Board;
class Property;
class Street;
class Space;

class PlayerActionGenerator {
public:
    // static std::vector<PlayerAction> generateActions(Player* player, Board& board, int diceRoll, bool isDoubles);
    using PopupFunction = std::function<void(const std::string&)>;

    static std::vector<PlayerAction> generateActions(Player* player, Board& board, int diceRoll, bool isDoubles, PopupFunction showPopup);


private:
    static void handlePropertyActions(Player* player, Property* property, std::vector<PlayerAction>& actions);
    static void handleStreetActions(Player* player, Street* street, std::vector<PlayerAction>& actions);
    static void handleSpecialSpaceActions(Player* player, Space* space, Board& board, std::vector<PlayerAction>& actions, PopupFunction showPopup);
    static void handleBuildingActions(Player* player, Board& board, std::vector<PlayerAction>& actions);
    // static void handleUtilityActions(Player* player, Railroad* property, std::vector<PlayerAction>& actions);
};