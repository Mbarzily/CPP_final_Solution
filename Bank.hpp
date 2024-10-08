#pragma once

class Player;

class Bank {
public:
    static int transfer(Player* from, Player* to, int amount);
    static int transferToBank(Player* from, int amount);
    static void transferFromBank(Player* to, int amount);
    static bool canTransfer(Player* from, int amount);

};