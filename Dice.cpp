#include "Dice.hpp"
#include <iostream>
#include <cmath>

std::random_device Dice::rd;
std::mt19937 Dice::gen(Dice::rd());
std::uniform_int_distribution<> Dice::dis(2, 3);

Dice::Dice() : animationFrames(0) {
    initializeDice();
}

void Dice::initializeDice() {
    // Initialize dice shapes
    for (int i = 0; i < 2; ++i) {
        diceShapes[i].setSize(sf::Vector2f(80, 80));
        diceShapes[i].setFillColor(sf::Color::White);
        diceShapes[i].setOutlineThickness(2);
        diceShapes[i].setOutlineColor(sf::Color::Black);
    }

    // Initialize dot shapes
    for (int i = 0; i < 14; ++i) {
        dots[i].setRadius(5);
        dots[i].setFillColor(sf::Color::Black);
    }

    // Set initial positions
    diceShapes[0].setPosition(400, 300);
    diceShapes[1].setPosition(520, 300);

    currentRoll = {1, 1};
    updateDots();
}

std::pair<int, int> Dice::roll() {
    return {dis(gen), dis(gen)};
}

bool Dice::isDoubles(const std::pair<int, int>& roll) {
    return roll.first == roll.second;
}

void Dice::animate(sf::RenderWindow& window, const std::pair<int, int>& roll) {
    finalRoll = roll;
    animationFrames = 30;  // 0.5 seconds at 60 FPS

    // Set positions for dice
    sf::Vector2u windowSize = window.getSize();
    diceShapes[0].setPosition(windowSize.x * 0.2f, windowSize.y * 0.3f);
    diceShapes[1].setPosition(windowSize.x * 0.4f, windowSize.y * 0.3f);
}

void Dice::update() {
    if (animationFrames > 0) {
        // Randomly change dice face during animation
        currentRoll = {dis(gen), dis(gen)};
        updateDots();

        animationFrames--;

        // Set final dice values when animation ends
        if (animationFrames == 0) {
            currentRoll = finalRoll;
            updateDots();
        }
    }
}

void Dice::updateDots() {
    for (int i = 0; i < 2; ++i) {
        sf::Vector2f dicePos = diceShapes[i].getPosition();
        int value = (i == 0) ? currentRoll.first : currentRoll.second;
        
        // Clear previous dots
        for (int j = 0; j < 7; ++j) {
            dots[i * 7 + j].setPosition(-100, -100);  // Move off-screen
        }

        // Set new dots based on dice value
        switch (value) {
            case 1:
                setDot(i, 0, 0.5, 0.5);
                break;
            case 2:
                setDot(i, 0, 0.25, 0.25);
                setDot(i, 1, 0.75, 0.75);
                break;
            case 3:
                setDot(i, 0, 0.25, 0.25);
                setDot(i, 1, 0.5, 0.5);
                setDot(i, 2, 0.75, 0.75);
                break;
            case 4:
                setDot(i, 0, 0.25, 0.25);
                setDot(i, 1, 0.25, 0.75);
                setDot(i, 2, 0.75, 0.25);
                setDot(i, 3, 0.75, 0.75);
                break;
            case 5:
                setDot(i, 0, 0.25, 0.25);
                setDot(i, 1, 0.25, 0.75);
                setDot(i, 2, 0.5, 0.5);
                setDot(i, 3, 0.75, 0.25);
                setDot(i, 4, 0.75, 0.75);
                break;
            case 6:
                setDot(i, 0, 0.25, 0.25);
                setDot(i, 1, 0.25, 0.5);
                setDot(i, 2, 0.25, 0.75);
                setDot(i, 3, 0.75, 0.25);
                setDot(i, 4, 0.75, 0.5);
                setDot(i, 5, 0.75, 0.75);
                break;
        }
    }
}

void Dice::setDot(int diceIndex, int dotIndex, float relativeX, float relativeY) {
    sf::Vector2f dicePos = diceShapes[diceIndex].getPosition();
    sf::Vector2f diceSize = diceShapes[diceIndex].getSize();
    dots[diceIndex * 7 + dotIndex].setPosition(
        dicePos.x + diceSize.x * relativeX - 5,
        dicePos.y + diceSize.y * relativeY - 5
    );
}

void Dice::draw(sf::RenderWindow& window) {
    for (const auto& diceShape : diceShapes) {
        window.draw(diceShape);
    }
    for (const auto& dot : dots) {
        window.draw(dot);
    }
}

std::pair<int, int> Dice::getCurrentRoll() const {
    return currentRoll;
}

bool Dice::isAnimating() const {
    return animationFrames > 0;
}
void Dice::setCurrentRoll(std::pair<int, int> rolly){
    currentRoll = rolly;
}
