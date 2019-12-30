#include "gameState.h"
#include <iostream>
    using std::cout;
//display game state and options
void GameState::display()
{
    cout << "\nGame state\n";
    cout << "Pot: $" << pot << "\n";
    if(!communityCards.empty()){
        cout << "Community cards:\n\t";
        displayCards(communityCards);
        cout << std::endl;
    }
}