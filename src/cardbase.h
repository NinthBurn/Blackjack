#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <cstring>
#include <windows.h>
/*struct card{
    int suit,type;
};*/
/*bool player_hascard(player &affected, int card)
{
    bool change=false;
    for(int i=0; i<4; ++i)
        for(int j=2; j<15; ++j)
            if(affected.cardtable[j+15*i] && (j+15*i)==card) change=1;
    return change;
}*/
std::string CARDTABLE[61]{ // H-hearts, D-diamonds, S-spades, C-clubs
    ">_<", "1-\u0003", "2-\u0003", "3-\u0003", "4-\u0003", "5-\u0003", "6-\u0003", "7-\u0003", "8-\u0003", "9-\u0003", "10\u0003", "J-\u0003", "Q-\u0003", "K-\u0003", "A-\u0003",
    "0-\u0004", "1-\u0004", "2-\u0004", "3-\u0004", "4-\u0004", "5-\u0004", "6-\u0004", "7-\u0004", "8-\u0004", "9-\u0004", "10\u0004", "J-\u0004", "Q-\u0004", "K-\u0004", "A-\u0004",
    "0-\u0006", "1-\u0006", "2-\u0006", "3-\u0006", "4-\u0006", "5-\u0006", "6-\u0006", "7-\u0006", "8-\u0006", "9-\u0006", "10\u0006", "J-\u0006", "Q-\u0006", "K-\u0006", "A-\u0006",
    "0-\u0005", "1-\u0005", "2-\u0005", "3-\u0005", "4-\u0005", "5-\u0005", "6-\u0005", "7-\u0005", "8-\u0005", "9-\u0005", "10\u0005", "J-\u0005", "Q-\u0005", "K-\u0005", "A-\u0005",
};
int CARDTABLE_V[61]{
    0,0,2,3,4,5,6,7,8,9,10,10,10,10,11,
    0,0,2,3,4,5,6,7,8,9,10,10,10,10,11,
    0,0,2,3,4,5,6,7,8,9,10,10,10,10,11,
    0,0,2,3,4,5,6,7,8,9,10,10,10,10,11
};
struct player{
    int chips,pcard[12],bet=0;
    bool cardtable[61]{false},canbet=true;
};
