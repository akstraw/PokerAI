#include "card.h"
#include <iostream>
    using std::cout;
    
Card::Card (Rank r, Suit s)
{
    rank = r;
    suit = s;
}
void Card::display ()
{
    if (rank <= 10)
        cout << rank;
    else
        cout << rankMap[rank];
    cout << " of " << suitMap[suit] << "s";
}
//strict equality (rank and suit)
bool operator== (const Card &leftCard, const Card &rightCard)
{
    bool rv = false;
    if (leftCard.rank == rightCard.rank && leftCard.suit == rightCard.suit)
        rv = true;
    return rv;
}
//only checks rank
bool operator<(const Card& leftCard, const Card& rightCard)
{
    bool rv = false;
    if (static_cast<int>(leftCard.rank) < static_cast<int>(rightCard.rank))
        rv = true;
    return rv;
}

//helper function
void displayCards (std::list<Card> cards)
{
    for(Card c : cards){
        c.display();
        if (true)
            cout << ", ";
    }
}