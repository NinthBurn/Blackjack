#include "cardbase.h"

bool GLOBALCARDTABLE[61]={false};
bool quitgame=false,holdbet=true;
int cardpos;
player table_player,dealer;

void resetcards(player &affected)
{
    for(int i=0; i<12; ++i)
        affected.pcard[i]=0;
    for(int i=0; i<61; ++i) affected.cardtable[i]=false,GLOBALCARDTABLE[i]=false;
    affected.bet=50;
}

bool card_available(int card)
{
    bool change=false;
    for(int i=0; i<4; ++i)
        for(int j=2; j<15; ++j)
            if(GLOBALCARDTABLE[j+15*i] && (j+15*i)==card) change=true;
    return change;
}

int calculate_points(player affected)
{
    int i,j,score=0;
    for(i=0; i<4; ++i)
        for(j=2; j<14; ++j)
            if(affected.cardtable[j+15*i])
                score+=CARDTABLE_V[j+15*i];
    for(i=0; i<4; ++i)
        if(affected.cardtable[14+15*i])
        {
            if((CARDTABLE_V[14+15*i]+score)<22)
                score+=CARDTABLE_V[14+15*i];
            else ++score;
        }
    return score;
}

void give_card(player &affected)
{
    bool ok=false;
    int carddealt=(rand()%60)+1;
    while(!ok)
    {
        bool change=0;
        for(int i=0; i<4; ++i)
            if(carddealt==i*15 || carddealt==(i*15+1))  change=1;
        if(!change) change=card_available(carddealt);
        if(change)  carddealt=(rand()%60)+1;
            else ok=1;
    }
    affected.pcard[cardpos]=carddealt, affected.cardtable[carddealt]=true;
    GLOBALCARDTABLE[carddealt]=true;
}

void start_gameinput()
{
    int gameinput;
    std::cout<<"Your input: ";
    std::cin>>gameinput;
    switch(gameinput)
    {
        case 1:
            table_player.chips=450;
            resetcards(dealer), resetcards(table_player);
            cardpos=1;
            table_player.bet=50;
            break;
        case 0:
            quitgame=true;
            break;
        default:
            std::cout<<"Wrong input! Try again.\n";
            start_gameinput();
            break;
    }
}

void calculate_bet(int mode)
{
    switch(mode)
    {
    default: // normal bet
        if(table_player.chips-table_player.bet>0)
        {
            table_player.chips-=table_player.bet;
            table_player.bet*=2;
        }else{
            table_player.bet+=table_player.chips;
            table_player.chips=table_player.canbet=0;
        }
        break;
    case 1: // double bet
        if(table_player.chips-table_player.bet*2>0)
        {
            table_player.chips-=table_player.bet*2;
            table_player.bet+=table_player.bet*2;
        }else{
            table_player.bet+=table_player.chips;
            table_player.chips=table_player.canbet=0;
        }
        break;
    }
}

void playerinput(int mode)
{
    int gameinput;
    std::cin>>gameinput;
    switch(mode)
    {
    case 1: // during game
        switch(gameinput)
        {
        case 0:
            quitgame=true;
            break;
        case 1:
            if(table_player.canbet) calculate_bet(0);
            holdbet=true;
            give_card(table_player);
            give_card(dealer);
            break;
        case 2:
            if(table_player.canbet)  calculate_bet(1);
            holdbet=true;
            give_card(table_player);
            give_card(dealer);
            break;
        case 3:
            if(holdbet)
            {
                give_card(dealer);
                if(table_player.canbet) holdbet=false;
            }
            else{
                holdbet=true;
                calculate_bet(0);
                give_card(table_player);
                give_card(dealer);
            }
            break;
        default:
            std::cout<<"Wrong imput! Try again: ";
            playerinput(1);
            break;
        }
        break;
    default: // after win/loss
        switch(gameinput)
        {
            default:
                resetcards(table_player);
                resetcards(dealer);
                break;
            case 0:
                quitgame=true;
                break;
        }
        break;
    }
}

int main(int argc, char** argv)
{
    wchar_t* windowtitle=L"  ♤   Blackjack v1.1 By NinthBurn   ♤";
    SetConsoleTitleW(windowtitle);
    std::cout<<"****--- b l a c k   j a c k ---****\nIn order to play the game, enter 1.\nTo exit, 0.\n";
    srand((unsigned) time(0));
    start_gameinput();
    if(argc>1)  table_player.chips=atoi(argv[1]);
    else table_player.chips=450;
    give_card(table_player);
    give_card(dealer);
    while(!quitgame){
        int pscore=calculate_points(table_player), dscore=calculate_points(dealer);
        ++cardpos;
        system("CLS");
        std::cout<<"Your cards are: ";
        for(int i=1; i<cardpos; ++i)    std::cout<<CARDTABLE[table_player.pcard[i]]<<" ";
        std::cout<<"or "<< pscore <<" points";
        std::cout<<"\nYou have $"<<table_player.chips<<" and your bet is $"<<table_player.bet;
        std::cout<<"\nDealer's cards: ";
        for(int i=1; i<cardpos; ++i)    std::cout<<CARDTABLE[dealer.pcard[i]]<<" ";
        std::cout<<"or "<< dscore <<" points";
        if(pscore>21){
            if(table_player.canbet)
            {
                std::cout<<"\nYou have lost this set! Press 1 to continue or 0 to quit the game.\nYour input: ";
                playerinput(0);
                table_player.chips-=50;
                table_player.bet=50;
                cardpos=1;
                give_card(table_player);
                give_card(dealer);
            }else{
                std::cout<< "\nYou've got no money left! Game over.";
                quitgame=true;
            }
        }else if((pscore<22 && dscore>21) || (pscore==21 && pscore!=dscore))
        {
            std::cout<<"\nYou have won $"<< table_player.bet <<"! Type any number to continue or 0 to quit the game.\nYour input: ";
            table_player.chips+=table_player.bet*2-50;
            table_player.bet=50;
            table_player.canbet=1;
            playerinput(0);
            cardpos=1;
            give_card(table_player);
            give_card(dealer);
        }else if(pscore==21 && dscore==21){
            std::cout<<"\nBoth players have reached 21 points, so the bet is returned.\nType any number to continue or 0 to quit the game.\nYour input: ";
            table_player.chips+=table_player.bet-50;
            table_player.bet=50;
            table_player.canbet=1;
            playerinput(0);
            cardpos=1;
            give_card(table_player);
            give_card(dealer);
        }else if(dscore==21 && pscore!=21){
            if(table_player.canbet)
            {
                std::cout<<"\nYou have lost this set! Press 1 to continue or 0 to quit the game.\nYour input: ";
                playerinput(0);
                table_player.chips-=50;
                table_player.bet=50;
                cardpos=1;
                give_card(table_player);
                give_card(dealer);
            }else{
                std::cout<< "\nYou've got no money left! Game over.";
                quitgame=true;
            }
        }else{
            if(table_player.canbet)
            {
                std::cout<<"\nYou can:\n 1.Bet the same amount\n 2.Bet double the amount\n 3.Hold bet (cannot hold consecutive bets; will be interpreted as same amount otherwise)\n 0.Quit the game\nYour input: ";
                playerinput(1);
            }else if(dscore==21){
                std::cout<< "\nYou've got no money left! Game over.";
                quitgame=true;
            }else if(dscore<21 && pscore>dscore){
                std::cout<< "\nYou've got no money left, but you can see if the dealer will bust.";
                std::cout<<"\nYou can:\n 1.Bet the same amount\n 2.Bet double the amount\n 3.Hold bet (cannot hold consecutive bets; will be interpreted as same amount otherwise)\n 0.Quit the game\nYour input: ";
                playerinput(1);
            }else quitgame=true;
        }
    }
    std::cout<<"\nThanks for playing! Press any key to close the game.";
    return 0;
}
