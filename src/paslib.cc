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
    int i = 0;
    for (auto &card: cards) { //just random setup for debugging - replace this
        switch (i) {
        case 0:
        case 1:
            pickPile.add(card);
            break;
        case 2:
            topPiles[0].add(card);
            break;
        case 3:
            topPiles[1].add(card);
            break;
        case 4:
            topPiles[2].add(card);
            break;
        case 5:
            topPiles[3].add(card);
            break;
        case 6:
            bottomPiles[0].add(card);
            break;
        case 7:
            bottomPiles[1].add(card);
            break;
        case 8:
            bottomPiles[2].add(card);
            break;
        case 9:
            bottomPiles[3].add(card);
            break;
        case 10:
            bottomPiles[4].add(card);
            break;
        case 11:
            bottomPiles[5].add(card);
            break;
        case 12:
            bottomPiles[6].add(card);
            break;
        default:
            i = -1;
        }
        i++;
    }
    pickPile.type = 0;
    dropPile.type = 1;
    for (auto &pile: topPiles) {
        pile.showTop();
        pile.type = 2;
    }
    for (auto &pile: bottomPiles) {
        pile.showTop();
        pile.type = 3;
    }

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


void Heap::add(vector<Card> &_cards)
{
    cards.insert(cards.end(), _cards.begin(), _cards.end());
}

void Heap::add(Card &card)
{
    cards.push_back(card);
}

void Heap::showTop()
{
    if (!cards.empty()) {
        cards.back().visible = true;
    }
}

void Game::load()
{
    Json::Value yolo;
    yolo["pasians"].append("Game1");

    cout << yolo << endl;
}
