#include "PlayerAction.hpp"

PlayerAction::PlayerAction(const std::string& description, std::function<void()> action)
    : description(description), action(action) {}

void PlayerAction::execute() {
    action();
}

std::string PlayerAction::getDescription() const {
    return description;
}
