#include "player.h"

#include <iostream>
    using std::cout;
    using std::cin;
#include <string>

int Player::count = 0;
//constructor, sets player number
Player::Player()
{
    number = ++count;
}
//when a player raises a bet
int Player::raise(int highestBet)
{
    int value =-1;
    do{
        cout << "Raise by: ";
        std::cin >> value;
        if (dollars < (value)){
            cout << "You do not have that much!\n";
            value = -1;
        }
        else if (dollars == (value)){
            cout << "All in!\n";
            allIn = true;
        }
        else if (value < highestBet){
            cout << "You must bet equal or more than highest bet!\n";
            value = -1;
        }
    }while (value <= 0);
    currentBet += value;
    dollars -= value;
    cout << "Player " << number << " raises by $" << value << std::endl;
    return value;
}
//when a player calls the highest bet
int Player::call(int highestBet)
{
    int rv = 0; //returns amount bet
    if (highestBet - currentBet <= (dollars))
        rv = highestBet - currentBet;
    else if ((highestBet - currentBet) > dollars)
        rv = dollars;
    dollars -= rv;
    currentBet += rv;
    cout << "Player " << number << " calls $" << rv << std::endl;
    if (dollars == 0){
        allIn = true;
        cout << "All in!\n";
    }
    return rv;
}
//player doesnt need to call and doesnt want to raise
void Player::check()
{
    cout << "Player " << number << " checks" << std::endl;
}
void Player::fold()
{
    cout << "Player " << number << " folds" << std::endl;
    in = false;
}
//when a player antes up (minimun bet on each round)
int Player::anteUp(int ante)
{
    dollarsPreRound = dollars;
    allIn = false;
    int rv = 0; //amount bet (ante, all-in, or nothing if a player is out)
    if (dollars > 0){
        in = true;
        if (dollars >= ante)
            rv = ante;
        else
            rv = dollars;
    }
    else in = false;
    currentBet = rv;
    dollars -= rv;
    if (dollars == 0 && in){
        allIn = true;
        cout << "All in!\n";
    }
    return rv;
}

int Player::foldCallRaise(GameState& gamestate)
{
    int bet = 0;
    int choice = 0;
    do{
        cout << "[1] View your cards\n";
        cout << "[2] Call\n";
        cout << "[3] Fold\n";
        cout << "[4] Raise\n";
        cin >> choice;
        if(choice == 1){
            cout << "Your cards are: ";
            displayCards(hole);
            cout << std::endl;
        }
        else if (choice == 2){
            bet = call(gamestate.highestBet);
        }
        else if (choice == 3){
            fold();
        }
        else if (choice == 4){
            bet = raise(gamestate.highestBet);
        }
        else{
            choice = 0;
            cin.clear();
            std::string trash;
            getline(cin, trash);
            cout << "Invalid input!\n";
        }
            
        }while (!choice || choice == 1);
    return bet;
}
int Player::foldOrCall(GameState& gamestate)
{
    int bet = 0;
    int choice = 0;
    do{
        cout << "[1] View your cards\n";
        cout << "[2] Call\n";
        cout << "[3] Fold\n";
        cin >> choice;
        if(choice == 1){
            cout << "Your cards are: ";
            displayCards(hole);
            cout << std::endl;
        }
        else if (choice == 2){
            bet = call(gamestate.highestBet);
        }
        else if (choice == 3){
            fold();
        }
        else{
            choice = 0;
            cin.clear();
            std::string trash;
            getline(cin, trash);
            cout << "Invalid input!\n";
        }
            
        }while (!choice || choice == 1);
    return bet;
}
int Player::checkOrRaise(GameState& gamestate)
{
    int bet = 0;
    int choice = 0;
    do{
        cout << "[1] View your cards\n";
        cout << "[2] Check\n";
        cout << "[3] Raise\n";
        cin >> choice;
        if(choice == 1){
            cout << "Your cards are: ";
            displayCards(hole);
            cout << std::endl;
        }
        else if (choice == 2){
            check();
        }
        else if (choice == 3){
            bet = raise(gamestate.highestBet);
        }
        else{
            choice = 0;
            cin.clear();
            std::string trash;
            getline(cin, trash);
            cout << "Invalid input!\n";
        }
            
        }while (!choice || choice == 1);
    return bet;
}
