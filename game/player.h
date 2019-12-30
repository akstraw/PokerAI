
#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "gameState.h"
#include <list>

class Player
{
    public:
        Player();
        //virtual ~Player(){}
        virtual void destruct () {}
        int call(int highestBet);
        void check();
        void fold();
        int anteUp(int ante);
        unsigned int number;  //player number
        int dollars;
        int currentBet = 0; // your money that's in the pot
        std::list<Card> hole;
        bool in = true;
        bool allIn = false;
        bool moved = false;
        bool winner = false;
        virtual int raise(int highestBet);
        virtual int foldCallRaise(GameState& gamestate);
        virtual int foldOrCall(GameState& gamestate);
        virtual int checkOrRaise(GameState& gamestate);
        virtual void updateDecisions(){}//used by ai
        static int count;
        int dollarsPreRound;
};

#endif