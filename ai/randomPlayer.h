
#ifndef RANDOMPLAYER_H
#define RANDOMPLAYER_H

#include "../game/card.h"
#include "../game/player.h"
#include <list>
#include "../game/gameState.h"

class RandomPlayer : public Player
{
    public:
        RandomPlayer();//srand(time(0));}
        int raise(int highestBet);
        //decision methods
        int foldCallRaise(GameState& gamestate);
        int foldOrCall(GameState& gamestate);
        int checkOrRaise(GameState& gamestate);
};

#endif