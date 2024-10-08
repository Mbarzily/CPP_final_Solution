#include "Bank.hpp"
#include "Player.hpp"
#include <iostream>

bool Bank::canTransfer(Player* from, int amount){
    return (from->getMoney() - amount) > 0;
    std::cout << from->getName() << " has left  " << from->getMoney() << " shekels. check amount of  " << amount << "." << std::endl;


}

// int Bank::transfer(Player* from, Player* to, int amount){
//     int leftovers = amount;
//     if(!canTransfer(from ,amount)){
//         leftovers = from->getMoney();
//         from->bankrupt(to);    
//     }
//     from->removeMoney(leftovers);
//     to->addMoney(leftovers);
//     return !(leftovers == amount);
    
// }
int Bank::transfer(Player* from, Player* to, int amount){
    if(!canTransfer(from ,amount)){
        from->bankrupt(to);
        return 0;    
    }
    else{
        from->removeMoney(amount);
        to->addMoney(amount);
        return 1;
    }
    
}

int Bank::transferToBank(Player* from, int amount){
    if(canTransfer(from ,amount)){
        from->removeMoney(amount);
        std::cout << from->getName() << " transferToBank: " << amount << std::endl;
        std::cout << from->getName() << " has left: " << from->getMoney() << std::endl;

        return 0;
    }
    from->bankrupt(nullptr);
    std::cout << from->getName() << " bankrupt: Too Bad. your money is:  " << from->getMoney() << std::endl;

    return -1;
}

void Bank::transferFromBank(Player* to, int amount){
    to->addMoney(amount);
}
