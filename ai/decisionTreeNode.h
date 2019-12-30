#ifndef DECISIONTREENODE_H
#define DECISIONTREENODE_H

#include <list>
#include <fstream>
#include <memory>
#include <vector>

#include "../game/myEnums.h"
//enum Round {DEAL = 0, FLOP, TURN, RIVER};
//enum DecisionType {FOLDORNOT = 0, FOLDORCALL, CALLORRAISE, CHECKORRAISE, RAISEBY};
//enum WeightType {LEFT = 0, RIGHT};//, RANK, HAND, MONEY, BET, POT, BLUFF};


class DecisionTreeNode
{
    public:
        static const int NUMWEIGHTS = 2;
        DecisionTreeNode(std::istream& fin);
        DecisionTreeNode(DecisionType type, int l, int r){
            decisionType = type; weights[LEFT] = l; weights[RIGHT] = r;
        }
        ~DecisionTreeNode();
        //the type of decision this node represents
        DecisionType decisionType;
        //record of winnings by each decison
        long weights [NUMWEIGHTS];
        // all proportions excluding left and right add up to 1. left and right add up to 100%
        double proportions [NUMWEIGHTS];
        std::unique_ptr<DecisionTreeNode> nextDecision;
        //methods
        bool write(std::ofstream& fout);
        void resetWeights();
        //returns list of new weights
        void add (DecisionTreeNode &n2);
        
};

#endif