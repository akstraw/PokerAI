#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H

#include "../game/card.h"
#include "../game/player.h"
#include "../game/handChecker.h"
#include "../game/gameState.h"
#include <list>
#include <array>
#include <vector>
#include <memory>
#include "../game/myEnums.h"
#include "decisionTree.h"

class ComputerPlayer : public Player
{
    public:
        ComputerPlayer();//constructor
        void destruct();//destructor
        int raise(int highestBet);
        int raise(const GameState& gamestate);
        //decision methods
        int foldCallRaise(GameState& gamestate);
        int foldOrCall(GameState& gamestate);
        int checkOrRaise(GameState& gamestate);

        std::list<DecisionTreeNode*> decisionsMade; //keeps track of which nodes traveled to for each round
        std::list<int> leftOrRightOnDecisionMade;
        std::vector<double> percentageize(const GameState& gamestate);
        void updateDecisions();
        DecisionTreeNode* getCurrentDec(const GameState& gamestate, int optionSetIndex);
        int indexPotPercent(const GameState& g);
        DecisionTree decTree;   //owned decision tree used for reading and writing to
};

/*
 *approximate probablity drawing a hand of this value or higher in texas holdem
 *from https://en.wikipedia.org/wiki/Poker_probability
 * used by raise function
*/
static std::map<Hand, double> handChances = {{HIGH, 100}, {PAIR, 82.6}, {TWOPAIR, 28.8},
    {THREEOFAKIND, 15.3}, {STRAIGHT, 10.4}, {FLUSH, 5.82},
    {FULLHOUSE, 2.8}, {FOUROFAKIND, .199}, {STRAIGHTFLUSH, .0311}, {ROYALFLUSH, .0032}};


#endif