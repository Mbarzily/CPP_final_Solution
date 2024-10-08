#include "Game.hpp"

int main() {
    Game& monopoly = Game::getInstance(4);  // Start a game with 4 players
    monopoly.run();
    return 0;
}
