# Monopoly Game Implementation

## Project Overview

This project is a C++ implementation of the classic board game Monopoly. It features a graphical user interface using SFML and implements core game mechanics including property management, player turns, dice rolling, and bankruptcy handling.

## Table of Contents

1. [Features](#features)
2. [Requirements](#requirements)
3. [Installation](#installation)
4. [Project Structure](#project-structure)
5. [Object-Oriented Programming in Monopoly Game](#object-oriented-programming-in-monopoly-game)
6. [Testing](#testing)
7. [License](#license)

## Features

- Graphical user interface using SFML
- Support for 2-8 players
- Full implementation of Monopoly game rules
- Property management (buying, selling, mortgaging)
- House and hotel building
- Chance and Community Chest cards
- Jail mechanics
- Bankruptcy handling
- Dice rolling with doubles logic

## Requirements

- C++17 compatible compiler
- SFML 2.5 or higher
- doctest (for unit testing)

## Installation

1. Clone the repository:
   ```
   git clone https://github.com/Mbarzily/CPP_final_Solution.git
   cd monopoly-game
   ```

2. Create a build directory and navigate to it:
   ```
   mkdir build && cd build
   ```

3. Generate the build files using Make:
   ```
   make ..
   ```

4. Run the game:
   ```
   ./monopoly
   ```

5. Run the test:
   ```
   ./monopoly_tests
   ```


## Project Structure

```
monopoly-game/
│
├── src/
│   ├── Main.cpp
│   ├── Game.cpp
│   ├── Player.cpp
│   ├── Board.cpp
│   ├── Property.cpp
│   ├── Street.cpp
│   ├── Utility.cpp
│   ├── Dice.cpp
│   ├── Bank.cpp
│   ├── Jail.cpp
│   ├── SpecialCard.cpp
│   └── PlayerActionGenerator.cpp
│
├── include/
│   ├── Game.hpp
│   ├── Player.hpp
│   ├── Board.hpp
│   ├── Property.hpp
│   ├── Street.hpp
│   ├── Utility.hpp
│   ├── Dice.hpp
│   ├── Bank.hpp
│   ├── Jail.hpp
│   ├── SpecialCard.hpp
│   └── PlayerActionGenerator.hpp
│
├── tests/
│   └── tests.cpp
│
├── assets/
│   ├── board.jpg
│   ├── arial.ttf
│   └── house.png
│
└── README.md
```



# Object-Oriented Programming in Monopoly Game

The Monopoly game implementation demonstrates several key OOP principles:

1. Encapsulation
2. Inheritance
3. Polymorphism
4. Abstraction

## 1. Encapsulation

Encapsulation is evident in how each class encapsulates its data and behavior:

- The `Game` class encapsulates the overall game state and logic.
- `Player` class encapsulates player-specific data (money, properties) and actions.
- `Board` class encapsulates the game board structure and spaces.

Example from `Player` class:
```cpp
class Player {
private:
    std::string name;
    int money;
    std::vector<Property*> properties;

public:
    void addMoney(int amount);
    bool removeMoney(int amount);
    void addProperty(Property* property);
    // ... other public methods ...
};
```

## 2. Inheritance

Inheritance is used to create specialized classes from more general ones:

- `Property` is a base class for ownable spaces.
- `Street`, `Utility`, and `Railroad` inherit from `Property`.
- All space types (including `BasicSpace`, `Jail`) inherit from a base `Space` class.

Example:
```cpp
class Property : public Space {
    // ... Property-specific members and methods ...
};

class Street : public Property {
    // ... Street-specific members and methods ...
};
```

## 3. Polymorphism

Polymorphism is utilized through virtual functions and runtime type identification:

- The `Space` class might have a virtual `landOn` method, which is overridden by its subclasses.
- The `Board` class can treat all spaces uniformly while allowing for type-specific behavior.

Example:
```cpp
class Space {
public:
    virtual void landOn(Player* player) = 0;
};

class Property : public Space {
public:
    void landOn(Player* player) override {
        // Property-specific landing behavior
    }
};
```

## 4. Abstraction

Abstraction is used to hide complex implementation details:

- The `Bank` class abstracts away the details of financial transactions.
- `Dice` class abstracts the dice rolling mechanism.
- `PlayerActionGenerator` abstracts the logic for determining available player actions.

Example:
```cpp
class Bank {
public:
    static void transfer(Player* from, Player* to, int amount);
    static void transferToBank(Player* player, int amount);
    static void transferFromBank(Player* player, int amount);
};
```

## Design Patterns

The game also utilizes some common design patterns:

1. Singleton Pattern: Used for the `Game` and `Board` classes to ensure only one instance exists.

2. Strategy Pattern: Potentially used in how different spaces handle being landed on.

3. Factory Pattern: Could be used for creating different types of spaces or cards.

## Composition

Composition is used to build complex objects from simpler ones:

- The `Board` is composed of various `Space` objects.
- The `Game` is composed of `Player` objects, a `Board`, and other game elements.

This OOP structure allows for:
- Easy extension (e.g., adding new property types or special spaces)
- Code reusability
- Improved maintainability
- Clear separation of concerns among different game components


## Testing

The project uses doctest for unit testing. To run the tests:

1. Navigate to the build directory.
2. Run the test executable:
   ```
   ./monopoly_tests
   ```

## License

[Specify your license here, e.g., MIT License, GPL, etc.]

