
#ifndef DECK_H
#define DECK_H

#include <vector>
#include "card.h"

class Deck
{
    public:
        void shuffle();
        Card draw();
        void reset();
        Deck(){reset();}
    private:
        void fill();
        std::vector<Card> cards;
};

#endif