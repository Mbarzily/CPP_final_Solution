# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -g -std=c++17

# SFML libraries
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Source files for the main game
MAIN_SRCS = Main.cpp Game.cpp Player.cpp Property.cpp Dice.cpp Street.cpp \
       PlayerActionGenerator.cpp PlayerAction.cpp Board.cpp Bank.cpp \
       Jail.cpp SpecialCard.cpp Utility.cpp

# Source files for tests (excluding Main.cpp)
TEST_SRCS = Game.cpp Player.cpp Property.cpp Dice.cpp Street.cpp \
       PlayerActionGenerator.cpp PlayerAction.cpp Board.cpp Bank.cpp \
       Jail.cpp SpecialCard.cpp Utility.cpp tests.cpp

# Object files
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# Main targets
all: monopoly monopoly_tests

monopoly: $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

monopoly_tests: $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Rule for building object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(MAIN_OBJS) $(TEST_OBJS) monopoly monopoly_tests

# Phony targets
.PHONY: all clean