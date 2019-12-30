
#ifndef GAME_H
#define GAME_H

#include <list>
#include "myEnums.h"
#include <vector>
#include <memory>
#include "deck.h"
#include "player.h"
#include "card.h"
#include "../ai/computerPlayer.h"
#include "../ai/randomPlayer.h"
#include "gameState.h"

class Game
{
    public:
        Game(bool ML = true, int numPlayers = 6); //constructor
        ~Game();
        //methods
        int round(); //begins a hand returns winner or -1
        void bettingRound(); //a round of betting/ returns whether a raise happened
        bool options (Player& p); //options for player on a bet
        void display ();    //displays game state;
        void deal (); // deals 2 cards to each player
        void flop(); // adds 3 cards to community
        void turn(); //adds foruth card to community
        void river();//fifth and  last card to community
        void showdown(); //see who won the hand
        int testRemaining(); //see if more than one player left
        
        //members
        GameState gamestate;
        std::vector<std::unique_ptr<Player>> players;
        Deck deck;
        //std::list<Card> communityCards;

    private:
        
        unsigned int button = 0;
        unsigned int currentPlayer;
};

#endif