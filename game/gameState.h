
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <list>
#include "card.h"
#include "../ai/decisionTree.h"
#include "myEnums.h"

class GameState
{
    public:
        GameState(){} //constructor
        void display ();    //displays game state;
        Round round;
        std::list<Card> communityCards;
        int highestBet = 0;
        int ante;
        int pot = 0;
        int playersIn;
        int totalMoney;
        int roundsPlayed = 0;
        bool ML;
};

#endif