#ifndef CARD_H
#define CARD_H

#include <map>
#include <string>
#include <list>
#include "myEnums.h"

class Card
{
    public:
        Card (Rank r, Suit s);
        Rank rank;
        Suit suit;
        void display ();
        friend bool operator== (const Card &leftCard, const Card &rightCard);   //strict equality (rank and suit)
        friend bool operator<(const Card& leftCard, const Card& rightCard);   //only checks rank
    private:
        std::map<Rank, std::string> rankMap = {{JACK, "Jack"}, {QUEEN, "Queen"}, {KING, "King"}, {ACE, "Ace"}};
        std::map<Suit, std::string> suitMap = {{CLUB, "Club"}, {DIAMOND, "Diamond"}, {HEART, "Heart"}, {SPADE, "Spade"}};
    
};

void displayCards (std::list<Card> cards);

#endif