#include "computerPlayer.h"
#include <chrono>
#include <random>
#include <iostream>
    using std::cout;
    using std::cin;
    using std::endl;
#include <unistd.h>
#include <utility>
#include <fstream>
    using std::ofstream;
    using std::ifstream;

//constructor reads decision tree then resets weights so they can record decisions made throughout the game
ComputerPlayer::ComputerPlayer() : Player{}
{
    decTree.read();
    decTree.resetWeights();
}
//random raise
int ComputerPlayer::raise(int highestBet)
{
    std::cerr << "Error: wrong raise function called\n";
    exit(1);
    int rv = highestBet;
    int bet = rand() % dollars;
    if (bet > highestBet) rv = bet;
    std::cout << "Player " << number << " raises " << rv << std::endl;
    currentBet += rv;
    dollars -= rv;
    return rv;
    
}
//raise a percentage of possible money bases on game state
int ComputerPlayer::raise(const GameState& gamestate)
{
    int bet = gamestate.highestBet;
    int betRange = dollars - gamestate.highestBet;
    std::vector<double> holdings = percentageize(gamestate);
    //calulate confidence for fold or not
    double confidence = 0;
    //indexes are rank hand money bluff
    confidence += holdings[0] * 0.25;
    if(gamestate.round != DEAL)
        confidence += holdings[1] * 0.35;
    else
        confidence += holdings[1] * 0.10;
    confidence += holdings[2] * 0.30;
    confidence += holdings[3] * 0.10;
    bet += betRange * (confidence / 100.0);
    currentBet += bet;
    dollars -= bet;
    if (dollars == 0){
        allIn = true;
        cout << "All in!\n";
    }
    std::cout << "Player " << number << " raises " << bet << std::endl;
    return bet;
}
//presents these three options to the computer
int ComputerPlayer::foldCallRaise(GameState& gamestate)
{
    const int OPTIONSETINDEX = 0;
    int rv = 0;
    int random = rand() % 100;
    DecisionTreeNode* currentDecision = getCurrentDec(gamestate, OPTIONSETINDEX);
    decisionsMade.push_front(currentDecision);   //record this decision
    if (random < currentDecision->proportions[LEFT]){
        fold();
        leftOrRightOnDecisionMade.push_front(LEFT);
    }
    else{
        leftOrRightOnDecisionMade.push_front(RIGHT);
        random = rand() % 100;
        currentDecision = currentDecision->nextDecision.get();
        decisionsMade.push_front(currentDecision);   //record this decision
        if(random < currentDecision->proportions[LEFT]){
            rv = call(gamestate.highestBet);
            leftOrRightOnDecisionMade.push_front(LEFT);
        }
        else{
            rv = raise(gamestate);
            leftOrRightOnDecisionMade.push_front(RIGHT);
        }
    }
    return rv;
}
int ComputerPlayer::foldOrCall(GameState& gamestate)
{
    int rv = 0;
    const int OPTIONSETINDEX = 1;
    int random = rand() % 100;
    DecisionTreeNode* currentDecision = getCurrentDec(gamestate, OPTIONSETINDEX);
    decisionsMade.push_front(currentDecision);   //record this decision
    if(random < currentDecision->proportions[LEFT]){
        fold();
        leftOrRightOnDecisionMade.push_front(LEFT);
    }
    else{
        rv = call(gamestate.highestBet);
        leftOrRightOnDecisionMade.push_front(RIGHT);
    }
    return rv;
}
int ComputerPlayer::checkOrRaise(GameState& gamestate)
{
    int rv = 0;
    const int OPTIONSETINDEX = 2;
    int random = rand() % 100;
    DecisionTreeNode* currentDecision = getCurrentDec(gamestate, OPTIONSETINDEX);
    decisionsMade.push_front(currentDecision);   //record this decision
    if(random < currentDecision->proportions[LEFT]){
        check();
        leftOrRightOnDecisionMade.push_front(LEFT);
    }
    else{
        rv = raise(gamestate);
        leftOrRightOnDecisionMade.push_front(RIGHT);
    }
    return rv;
}
//create percentage to be multiplyed by each weight. will range from 0-1 with 0 representing low confidence and 1 being high confidence
std::vector<double> ComputerPlayer::percentageize(const GameState& gamestate)
{
    const int maxHandRankSum = 65;
    const int maxDealRankSum = 26;
    double rankPercentage, handPercentage, moneyPercent, bluffPercent;
    std::vector<double> retvals;
    //hand and rank values
    if(gamestate.round == DEAL){
        rankPercentage = static_cast<int>(hole.front().rank) + static_cast<int>(hole.back().rank) * 1.0 / maxDealRankSum;
        handPercentage = rankPercentage;
    }
    else{
        std::list<Card> cards = hole;
        cards.insert(cards.end(), gamestate.communityCards.begin(), gamestate.communityCards.end());
        HandChecker hand{cards};
        hand.fiveHand();
        handPercentage = (100.0 - handChances[hand.hand]) / 100.0;
        int handsum = 0;
        for(Card c : hand.cards)
            handsum += static_cast<int>(c.rank);
        rankPercentage = handsum / maxHandRankSum;
    }
    
    moneyPercent = 100.0 * dollars / gamestate.totalMoney;
    bluffPercent = rand() % 100 / 1.0;
    retvals.push_back(rankPercentage);
    retvals.push_back(handPercentage);
    retvals.push_back(moneyPercent);
    retvals.push_back(bluffPercent);
    return retvals;
}
void ComputerPlayer::updateDecisions()
{
    for(DecisionTreeNode*& dec : decisionsMade)
    {
        dec->weights[leftOrRightOnDecisionMade.front()] += 10;
        leftOrRightOnDecisionMade.pop_front();
    }
    decisionsMade.clear();
    leftOrRightOnDecisionMade.clear();
}
//retrieves current decision from the tree
DecisionTreeNode* ComputerPlayer::getCurrentDec(const GameState& gamestate, int optionSetIndex)
{
    DecisionTreeNode* currentDecision;
    int potIndex = indexPotPercent(gamestate);
    int opponentIndex = gamestate.playersIn - 2;
    if(gamestate.round == DEAL){
        int highCard = 0;
        for(Card& c : hole){
            if(static_cast<int>(c.rank)-2 > highCard){
                highCard = static_cast<int>(c.rank)-2;
                highCard/=2;
                if(highCard>5) highCard = 5;
            }
        }
        currentDecision = decTree.dealTree[highCard][potIndex][opponentIndex][optionSetIndex].get();
    }   
    else{
        std::list<Card> cards = hole;
        cards.insert(cards.end(), gamestate.communityCards.begin(), gamestate.communityCards.end());
        HandChecker hand{cards};
        hand.fiveHand();
        int highCardIndex = (hand.highCard-2)/2;
        if(highCardIndex>5) highCardIndex = 5;
        int handIndex = hand.hand;
        int holeIndex = 0;
        for(Card& c : hand.cards){
            if(c == hole.front() || c == hole.back())
                holeIndex++;
        }
        if(gamestate.round == FLOP){
            currentDecision = decTree.flopTree[handIndex][highCardIndex][potIndex][opponentIndex][optionSetIndex].get();
        }
        else if (gamestate.round == TURN){
            currentDecision = decTree.turnTree[holeIndex-1][handIndex][highCardIndex][potIndex][opponentIndex][optionSetIndex].get();
        }
        else{
            currentDecision = decTree.riverTree[holeIndex][handIndex][highCardIndex][potIndex][opponentIndex][optionSetIndex].get();
        }
    }
    return currentDecision;
}
//indexes percentage of total moeny in pot at 'significant' points sections are <(5 10 15 20 30 40 50 65 75 100)%
int ComputerPlayer::indexPotPercent(const GameState& g)
{
    double percent = 100.0 * g.pot / g.totalMoney;
    int rv = 0;
    if(percent < 5)
        rv = 0;
    else if(percent < 10)
        rv = 1;
    else if(percent < 15)
        rv = 2;
    else if(percent < 20)
        rv = 3;
    else if(percent < 30)
        rv = 4;
    else if(percent < 40)
        rv = 5;
    else if(percent < 50)
        rv = 6;
    else if(percent < 65)
        rv = 7;
    else if(percent < 75)
        rv = 8;
    else
        rv = 9;
    return rv;
}
void ComputerPlayer::destruct()//destructor
{
    if(winner){
        DecisionTree oldTree;
        oldTree.read();
        oldTree.sum(decTree);
        oldTree.write();
    }
}
