#pragma once
#include <string>
#include <functional>

class PlayerAction {
public:
    PlayerAction(const std::string& description, std::function<void()> action);
    void execute();
    std::string getDescription() const;

private:
    std::string description;
    std::function<void()> action;
};