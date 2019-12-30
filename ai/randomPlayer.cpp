#include "randomPlayer.h"
#include <chrono>
#include <random>
#include <iostream>
    using std::cout;
    using std::cin;
    using std::endl;
#include <fstream>
    using std::ofstream;
    using std::ifstream;
RandomPlayer::RandomPlayer() : Player{}
{}
int RandomPlayer::raise(int highestBet)
{
    int rv = highestBet;
    int bet = rand() % dollars;
    if (bet > highestBet) rv = bet;
    std::cout << "Player " << number << " raises " << rv << std::endl;
    currentBet += rv;
    dollars -= rv;
    return rv;
    
}

int RandomPlayer::foldCallRaise(GameState& gamestate)
{
    int rv = 0;
    for(Card c : gamestate.communityCards)
        rv++;
    rv = 0;
    int confidence = rand() % 2;
    if (confidence == 0)
        fold();
    else{
        confidence = rand() % 2;
        if  (confidence == 0)
            call(gamestate.highestBet);
        else
            rv = raise(gamestate.highestBet);
    }
    return rv;
}
int RandomPlayer::foldOrCall(GameState& gamestate)
{
    int rv = 0;
    for(Card c : gamestate.communityCards)
        rv++;
    rv = 0;
    int confidence = rand() % 2;
    if (confidence == 0)
        fold();
    else
        rv = call(gamestate.highestBet);
    return rv;
}
int RandomPlayer::checkOrRaise(GameState& gamestate)
{
    int rv = 0;
    for(Card c : gamestate.communityCards)
        rv++;
    rv = 0;
    int confidence = rand() % 2;
    if (confidence == 0)
        check();
    else
        rv = raise(gamestate.highestBet);
    return rv;
}