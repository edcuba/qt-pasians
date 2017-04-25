/**
* paslib.cc
* Pasians library - ICP 2016/2017
* Authors:
*   Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
*   Ondrej Kurak <xkurak00@stud.fit.vutbr.cz>
**/

#include <iostream>
#include "paslib.h"

using namespace std;

Card::Card(char _type, char _color)
{
    type = _type;
    color = _color;
}


void Game::setup()
{
    vector<Card> cards = getCards();
    //TODO: setup piles
    pickPile.add(cards);
    pickPile.showTop();
}

/**
* Generate vector of 52 play cards
* @return #vector of play cards, order is constant
**/
vector<Card> Game::getCards()
{
    vector<Card> pile;
    char c = 0;
    for (char t = 0; t < 52; ++t) {
        Card card(t % 13 + 2, c);
        pile.push_back(card);
        c++;
        c %= 4;
    }
    return pile;
}


void Heap::add(vector<Card> _cards)
{
    cards.insert(cards.end(), _cards.begin(), _cards.end());
}

void Heap::showTop()
{
    if (!cards.empty()) {
        cards.back().visible = true;
    }
}
