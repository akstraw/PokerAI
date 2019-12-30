#include <algorithm>
#include <iostream>
    using std::cout;
    using std::cin;
#include "game.h"
#include "handChecker.h"
#include "../ai/computerPlayer.h"
#include "../ai/randomPlayer.h"
#include <utility>
#include <fstream>
#include <string>
#include <sstream>

Game::Game(bool ML, int numPlayers) //constructor
{
    //arbitrarily set ante
    gamestate.ante = 5;//(rand() % 5) * 5;
    cout << "Ante set to $" << gamestate.ante << std::endl; 
    //add players
    gamestate.ML = ML;
    #pragma omp critical
    {
        Player::count = 0;
        if(ML)
            players.push_back(std::move(std::unique_ptr<Player>{new ComputerPlayer{}}));
        else
            players.push_back(std::unique_ptr<Player>{new Player{}});
        for(int i = 1; i < numPlayers; i++)
            players.push_back(std::move(std::unique_ptr<Player>{new ComputerPlayer{}}));
    }
    //set each players money
    const int DOLLARS = 500;
    for_each(players.begin(), players.end(),
            [](std::unique_ptr<Player>& p) {p->dollars = DOLLARS;});
    gamestate.totalMoney = DOLLARS * players.size();
    //do rounds until a player wins
    button = players.size() - 1;
    gamestate.playersIn = players.size();
    int winner = -1;
    while (winner == -1)
        winner = round();
    cout << "Player " << winner << " wins the game!!!!!\n";
    players[winner - 1]->winner = true;
    currentPlayer = winner - 1; // to index for destructor win records
   #pragma omp critical
    {//adds to decision tree from the winning player
        for(auto& p : players)
            p->destruct();
    }
    players.clear();
}
//begins a hand returns overall winner or -1
int Game::round()
{
    for(auto& p : players){
        if(p->allIn && p->dollars == 0)
            cout << "Player " << p->number << " is out" << std::endl;
        p->allIn = false;
    }
    if(gamestate.roundsPlayed + 1 % 10  == 0){  //double ante every 10 rounds
        gamestate.ante *= 2;
        if(!gamestate.ML) cout << "Ante set to $" << gamestate.ante << "!!!!!!!!!!\n" << std::endl; 
    }
    gamestate.communityCards.clear();
    gamestate.pot = 0;
    gamestate.highestBet = 0;
    if (button < players.size() - 1)  //rotate "dealer" (first player who bets in this case)
        button++;
    else
        button = 0;
    currentPlayer = button;
    //pulls ante and checks if players still in game
    for(auto& p : players)
        gamestate.pot += p->anteUp(gamestate.ante);
    if(testRemaining() == 1){
        currentPlayer = 1;
        while(players[currentPlayer-1]->in == false) //Find who won the overall game
            currentPlayer++;
        return currentPlayer; 
    }
    cout << "************** New round begins! **************\n";
    //deal
    deck.reset();
    cout << "Deal" << std::endl;
    deal();
    currentPlayer = 0;
    if(testRemaining() > 1)
        flop();
    if(testRemaining() > 1)
        turn();
    if(testRemaining() > 1)
        river();
    if(testRemaining() > 1)
        showdown();
    else{
        currentPlayer = 0;
        while(players[currentPlayer]->in == false) //Find who won this round
            currentPlayer++;
        players[currentPlayer]->dollars += gamestate.pot;
        cout << "Player " << currentPlayer + 1 << " won this round!\n";
    }
    gamestate.roundsPlayed ++;
    for(auto& p : players)
        p->updateDecisions();
    return -1;//flag for no overall winner found yet
}
// round of betting for first deal, flop, turn, and river
void Game::bettingRound()
{
    //set all players bet to 0
    for(auto& p : players){
        p->currentBet = 0;
        p->moved = false;
    }
    gamestate.highestBet = 0;
    
    bool raised = true;
    while(raised)
    {
        raised = false;
        currentPlayer = button;  // first player to bet (button) rotates 
        do{
            //test # of players not all in
            bool anotherPlayerIn = false;
            if(testRemaining() > 1){
                for(auto& p : players)
                    if(p->number != currentPlayer+1 && p->in && !p->allIn)
                        anotherPlayerIn = true;
            }
            if(anotherPlayerIn){
                if(options(*players[currentPlayer])) raised = true;
            }
            if(currentPlayer  < players.size() - 1){
                currentPlayer++;
            }
            else
                currentPlayer = 0;
        }while (currentPlayer != button);
    }
    
}

//menu for player on betting turn
bool Game::options(Player& p) 
{
    bool raised = false;
    //you have options if your in, not all in, and (havent used your raise, or need to call someone elses raise)
    if(p.in && !p.allIn && (!p.moved || p.currentBet < gamestate.highestBet))
    {
        if (!gamestate.ML && p.number == 1) display();  //display if fleshbag
        int bet = 0;
        if(p.currentBet < gamestate.highestBet){ //call is an option
            if(!p.allIn && p.dollars > gamestate.highestBet && !p.moved) //raise is an option
                bet = p.foldCallRaise(gamestate);
            else
                bet = p.foldOrCall(gamestate);
        }
        else{
            bet = p.checkOrRaise(gamestate);
        }
        gamestate.pot += bet;
        if(bet > gamestate.highestBet){
            gamestate.highestBet = bet;
            raised = true;
        }
        p.moved = true;
    }
    return raised;
}
//display game state and options
void Game::display()
{
    gamestate.display();
    cout << "Current Player, player " << currentPlayer + 1 << " has "
         << players[currentPlayer]->dollars << " dollars\n";
}
// deals 2 cards to each player
void Game::deal () 
{
    gamestate.round = DEAL;
    for(auto& p : players)
    {
        if (p->in){
            p->hole.clear();
            p->hole.push_front(deck.draw());
            p->hole.push_front(deck.draw());
        }
    }
    //first round of betting
    bettingRound();
}
// adds 3 cards to community
void Game::flop() 
{
    gamestate.round = FLOP;
    if(!gamestate.ML)cout << "Flop!\n";
    for (int i = 0; i < 3; i++)
        gamestate.communityCards.push_front(deck.draw());
    bettingRound();
}
 //adds fourth card to community
void Game::turn()
{
    gamestate.round = TURN;
    if(!gamestate.ML) cout << "Turn!\n";
    gamestate.communityCards.push_front(deck.draw());
    bettingRound();
}
//fifth and  last card to community
void Game::river()
{
    gamestate.round = RIVER;
    if(!gamestate.ML) cout << "River!\n";
    gamestate.communityCards.push_front(deck.draw());
    bettingRound();
}
//see who won the hand
void Game::showdown() 
{
    cout << "Showdown!!!\n";
    HandChecker winningHand{};
    //loop to check all hands for overall best hand
    for (auto& p : players)
    {
        //if player is in construct their best hand from their hole and the community cards
        if (p->in)
        {
            //copies community cards to players hole. Hole is cleared on deal()
            p->hole.insert(p->hole.end(), gamestate.communityCards.begin(), gamestate.communityCards.end());
            HandChecker currentHand(p->hole);
            currentHand.fiveHand(); //reduces to 5 cards
            p->hole = currentHand.cards;
            if(!gamestate.ML){
                cout << "Player " << p->number << "'s hand is ";
                displayCards(p->hole);
                cout << std::endl;
            }
            //if current hand is better or hand is the same and high card is better then it is a winning hand
            if (winningHand < currentHand)
                winningHand = currentHand;
        }
    }
    std::list<int> winningPlayerNumbers{};
    //loop to find winning player(s) number(s)
    for(auto& p : players)
    {
        HandChecker hand{p->hole};
        if (p->in && hand == winningHand)
            winningPlayerNumbers.push_front(p->number);
    }
    //awards all winning player(s) a division of the pot
    cout << "Winning ";
    winningHand.display();  //"hand is ..."
    cout << "Player";
    if (winningPlayerNumbers.size() > 1) cout << "s";
    cout << " ";
    for(int winningNumber : winningPlayerNumbers){
        cout << winningNumber << ", ";
        players[winningNumber-1]->dollars += gamestate.pot / winningPlayerNumbers.size();
    }
    cout << "won this round!\n";
}
//see if more than one player left
int Game::testRemaining() 
{
    int count = 0;
    for (auto& p : players){
        if (p->in)
            count++;
    }
    return gamestate.playersIn = count;
}
Game::~Game()
{
    #pragma omp critical
    {
        std::ifstream fin{"game/gameStats.txt"};
        std::string line;
        getline(fin, line);
        fin.close();
        std::stringstream linestream{line};
        long gamesPlayed;
        long playerWins [6];
        linestream >> gamesPlayed;
        for (int i = 0; i < 6; i++)
            linestream >> playerWins[i];
        gamesPlayed++;
        playerWins[currentPlayer]++;
        std::ofstream fout{"game/gameStats.txt"};
        fout << gamesPlayed << " ";
        for (int i = 0; i < 6; i++)
            fout << playerWins[i] << " ";
        fout.close();
    }
}
