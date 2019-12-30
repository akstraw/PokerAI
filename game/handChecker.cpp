#include "handChecker.h"
#include "card.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

//constructor, checks for each hand highest to lowest
HandChecker::HandChecker (std::list<Card> newCards)
{
    cards = newCards;
    if(checkRoyalFlush())
        hand = ROYALFLUSH;
    else if (checkStraightFlush())
        hand = STRAIGHTFLUSH;
    else if (checkFourOfAKind())
        hand = FOUROFAKIND;
    else if (checkFullHouse())
        hand = FULLHOUSE;
    else if (checkFlush())
        hand = FLUSH;
    else if (checkStraight())
        hand = STRAIGHT;
    else if (checkThreeOfAKind())
        hand = THREEOFAKIND;
    else if (checkTwoPair())
        hand = TWOPAIR;
    else if (checkPair())
        hand = PAIR;
    else
        hand = HIGH;
    //display();
}
bool HandChecker::checkRoyalFlush ()
{
    bool rv = false;
    //check royal flush for each suit until one found
    for (int suit = CLUB; suit <= SPADE && !rv; suit++)
    {
        for(int rank = TEN; rank <= ACE
            && cardCounter(Card{static_cast<Rank>(rank), static_cast<Suit>(suit)}, true); rank++)
        {
            if (rank == ACE &&
                cardCounter(Card{static_cast<Rank>(rank), static_cast<Suit>(suit)}, true)){
                rv = true;
            }
        }
    }
    highCard = ACE;
    return rv;
}
bool HandChecker::checkStraightFlush ()
{
    bool rv = false;
    //for each card as beggining of straight
    cards.sort();
    int count = 1;
    Card lastCard{TWO, HEART};//will hold the last card
    for(Card c : cards)
    {
        if (!(c == cards.front()) && c.rank == lastCard.rank + 1 && c.suit == lastCard.suit)
            count++;
        else if (!(c == cards.front()))
            count = 1;
        lastCard = c;
        if (count == 5 ) rv = true;
    }
    if (count == 5 ) rv = true;
    return rv;
}
bool HandChecker::checkFourOfAKind()
{
    bool rv = false;
    for(Card c : cards)
        if (cardCounter(c) == 4){
            rv = true;
            highCard = c.rank;
        }
    return rv;
}
bool HandChecker::checkFullHouse ()
{
    int threeCounts = 0;
    int twoCounts = 0;
    bool rv = false;
    for(Card c : cards)
    {
        if (cardCounter(c) == 2)
            twoCounts++;
        else if (cardCounter(c) == 3){
            threeCounts++;
            if(c.rank > highCard) highCard = c.rank;
        }
    }
    if (threeCounts && (twoCounts || threeCounts > 3)) rv = true;
    return rv;
}
bool HandChecker::checkFlush ()
{
    bool rv = false;
    for (int suit = CLUB; suit <= SPADE; suit++)
    {
        int count = 0;
        for (Card c : cards)
            if (c.suit == suit) count++;
        if (count >= 5) rv = true;
    }
    return rv;
}
bool HandChecker::checkStraight ()
{
    bool rv = false;
    //for each card as beggining of straight
    cards.sort();
    int count = 1;
    Card lastCard{TWO, HEART};//will hold the last card
    for(Card c : cards)
    {
        if (!(c == cards.front()) && c.rank == lastCard.rank + 1)
            count++;
        else if (!(c == cards.front()))
            count = 1;
        lastCard = c;
        if (count == 5 ) rv = true;
    }
    if (count == 5 ) rv = true;
    return rv;
}
bool HandChecker::checkThreeOfAKind()
{
    bool rv = false;
    for(Card c : cards)
        if (cardCounter(c) == 3){
            rv = true;
            highCard = c.rank;
        }
    return rv;
}
bool HandChecker::checkTwoPair()
{
    bool rv = false;
    int twoCounts = 0;
    for(Card c : cards)
        if (cardCounter(c) == 2){
            twoCounts++;
            if(c.rank > highCard) highCard = c.rank;
        }
    if(twoCounts >= 4) rv = true;
    return rv;
}
bool HandChecker::checkPair()
{
    bool rv = false;
    for(Card c : cards){
        if (cardCounter(c) == 2){
            rv = true;
            highCard = c.rank;
        }
    }
    return rv;
}
//displays high hand
void HandChecker::display()
{
    std::cout << "hand is " << handMap[hand] << std::endl;
}
 //clears cards not neaded for hand
void HandChecker::fiveHand()
{
    if(cards.size() <= 5) return;
    cards.sort();
    std::list<Card> cardsCopy = cards;
    auto iterator = cardsCopy.begin();
    for(unsigned int currentCard = 0; currentCard < cards.size(); currentCard++)
    {
        cardsCopy = cards;
        iterator = cardsCopy.begin();
        if(currentCard) advance(iterator, currentCard);
        cardsCopy.erase(iterator);
        if(HandChecker{cardsCopy}.hand == hand)
            break;
    }
    cards = cardsCopy;
    if(cards.size() > 5)    //recursively call to remove another card
        fiveHand();
}
//counts for instances of a particular card
int HandChecker::cardCounter(Card card, bool checkSuit)
{
    int count = 0;
    for (Card c : cards)
    {
        if (c.rank == card.rank)
        {
            count++;
            if (checkSuit && c.suit != card.suit) count--;
        }
    }
    return count;
}
//      NEEDS LOGIC FOR 5 CARD RANKS
bool operator<(const HandChecker& leftHand, const HandChecker& rightHand)
{
    if(leftHand.cards.size() > 5) exit(1);    //reduce to needed 5 cards
    if(rightHand.cards.size() > 5) exit(1);
    if(leftHand.cards.size() < 5) return true;  //empty handchecker
    bool rv = false;
    if (static_cast<int>(leftHand.hand) < static_cast<int>(rightHand.hand))
        rv = true;
    else if (static_cast<int>(leftHand.hand) == static_cast<int>(rightHand.hand))
    {
        if((leftHand.hand == FOUROFAKIND || leftHand.hand == THREEOFAKIND || leftHand.hand == PAIR) && leftHand.highCard < rightHand.highCard)
            rv = true;
        else if ((leftHand.hand == FULLHOUSE || leftHand.hand == TWOPAIR) && (leftHand.highCard < rightHand.highCard))
            rv = true;
        else    //check all ranks 
        {
            std::vector<Card> leftCopy {leftHand.cards.begin(), leftHand.cards.end()};
            std::vector<Card> rightCopy {rightHand.cards.begin(), rightHand.cards.end()};
            for(int i =4; i >= 0; i--)
            {
                if(static_cast<int>(leftCopy[i].rank) < static_cast<int>(rightCopy[i].rank))
                    rv = true;
            }
        }
    }
    return rv;
}
bool operator== (const HandChecker &leftHand, const HandChecker &rightHand)
{
    bool rv = false;
    if (!(leftHand < rightHand || rightHand < leftHand))
        rv = true;
    return rv;
}