#include "deck.h"
#include "card.h"
#include <chrono>
#include <algorithm>
#include <random>

//create standard 52 card deck
void Deck::fill()
{
    for (int suit = CLUB; suit <= SPADE; suit++)
    {
        for(int rank = TWO; rank <= ACE; rank++)
        {
            cards.push_back( Card{ static_cast<Rank>(rank), static_cast<Suit>(suit)});
        }
    }
}
//empties and refills deck
void Deck::reset()
{
    cards.clear();
    fill();
    shuffle();
}
//draw card at top of deck
Card Deck::draw()
{
    Card rv = cards.back();
    cards.pop_back();
    return rv;
}
//shuffle cards in deck
void Deck::shuffle()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle ( cards.begin(), cards.end(),std::default_random_engine(seed));
}
