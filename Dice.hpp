#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <array>

class Dice {
public:
    Dice();
    static std::pair<int, int> roll();
    static bool isDoubles(const std::pair<int, int>& roll);
    void animate(sf::RenderWindow& window, const std::pair<int, int>& roll);
    void update();
    void draw(sf::RenderWindow& window);
    bool isAnimating() const;
    std::pair<int, int> getCurrentRoll() const;
    void setCurrentRoll(std::pair<int, int> roll);


private:
    static std::random_device rd;
    static std::mt19937 gen;
    static std::uniform_int_distribution<> dis;

    std::array<sf::RectangleShape, 2> diceShapes;
    std::array<sf::CircleShape, 14> dots;  // 7 dots per die, maximum

    void initializeDice();
    void updateDots();
    void setDot(int diceIndex, int dotIndex, float relativeX, float relativeY);

    int animationFrames;
    std::pair<int, int> currentRoll;
    std::pair<int, int> finalRoll;
};