#ifndef HANDCHECKER_H
#define HANDCHECKER_H

#include <map>
#include <string>
#include <list>
#include "card.h"
#include "myEnums.h"

class HandChecker
{
    public:
        //constructor, checks for each hand highest to lowest
        HandChecker (std::list<Card> newCards);
        //default constructor, creates hand with minimun value
        HandChecker (){}
        Hand hand;
        void display();
        void fiveHand(); //clears cards not neaded for hand
        //overloaded operators
        friend bool operator<(const HandChecker& leftHand, const HandChecker& rightHand);
        friend bool operator== (const HandChecker &leftHand, const HandChecker &rightHand);
        std::list<Card> cards;
        Rank highCard = TWO;  //used for instance by pair or three of a kind
    private:
        
        int cardCounter(Card card, bool checkSuit = false);
        bool checkRoyalFlush ();
        bool checkStraightFlush ();
        bool checkFourOfAKind();
        bool checkFullHouse ();
        bool checkFlush ();
        bool checkStraight ();
        bool checkThreeOfAKind();
        bool checkTwoPair();
        bool checkPair();
        std::map<Hand, std::string> handMap = {{HIGH, "High card"}, {PAIR, "Pair"}, {TWOPAIR, "Two pair"},
            {THREEOFAKIND, "Three of a kind"}, {STRAIGHT, "Straight"}, {FLUSH, "Flush"},
            {FULLHOUSE, "Full-house"}, {FOUROFAKIND, "Four of a kind"}, {STRAIGHTFLUSH, "Straight-Flush"}, {ROYALFLUSH, "Royal-Flush"}};
    
};

#endif