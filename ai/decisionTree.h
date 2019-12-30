#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "decisionTreeNode.h"
#include "../game/myEnums.h"
#include <list>
#include <array>
    using std::array;
#include <vector>
#include <memory>

class DecisionTree
{
    public:
        void read();
        void write();
        void resetWeights();
        void sum(DecisionTree& decTree);
        static const int MAXOPPONENTS = 5;  // represents max number of opponents in a game. is set to max of 6 players so 5 opponents
        static const int POSSIBLEOPTIONSETS = 3; // represents fold/call/raise, fold/call, or check/raise as possible options
        static const int POTPERCENTSECTIONS = 10;  //represents percentage of total money in the pot seperated into 10 sections
                                            //sections are <(5 10 15 20 30 40 50 65 75 100)%
        static const int NUMHANDS = 10;
        static const int HOLETURN = 2;     //representst 1 or 2 hole cards used in current hand
        static const int HOLERIVER = 3;   //represents 0, 1, or 2 hole cards used in current hand
        static const int TOTALRANKS = 6;  //2-Ace used to represent high card to the ai /2 to save space/time

        //decisions for each situation...
        array<array<array<array<std::unique_ptr<DecisionTreeNode>, POSSIBLEOPTIONSETS>, MAXOPPONENTS>, POTPERCENTSECTIONS>, TOTALRANKS> dealTree;
        array<array<array<array<array<std::unique_ptr<DecisionTreeNode>, POSSIBLEOPTIONSETS>, MAXOPPONENTS>,
            POTPERCENTSECTIONS>, TOTALRANKS>, NUMHANDS>  flopTree;
        array<array<array<array<array<array<std::unique_ptr<DecisionTreeNode>, POSSIBLEOPTIONSETS>, MAXOPPONENTS>,
            POTPERCENTSECTIONS>, TOTALRANKS>, NUMHANDS>, HOLETURN> turnTree;
        array<array<array<array<array<array<std::unique_ptr<DecisionTreeNode>, POSSIBLEOPTIONSETS>, MAXOPPONENTS>,
            POTPERCENTSECTIONS>, TOTALRANKS>, NUMHANDS>, HOLERIVER> riverTree;

};

#endif