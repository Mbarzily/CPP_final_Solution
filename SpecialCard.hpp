#include "Space.hpp"
#include "Card.hpp"
#include <vector>
#include <random>

class SpecialCard : public Space {
public:
    SpecialCard(const std::string& name, int position);
    void execute(Player* player);
    static SpecialCard getRandomCard();
    std::string getText() const { return name; }
    void landOn(Player* player) override;

private:
    static std::vector<SpecialCard> allCards;
    static void initializeCards();
};
    