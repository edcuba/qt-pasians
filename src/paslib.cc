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
    int i = 0;
    for (auto &card: cards)
    {
    if (i < 7)
    {
        for (int j=i; j < 7; ++j)
            bottomPiles[j].add(card);
    }
    else
    {
        pickPile.add(card);
    }
    ++i;
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

Move::Move(Pile *_from, Pile *_where)
{
    from = _from;
    where = _where;
}

int Game::move(Pile *from, Pile *where, int index)
{
    Move save(from, where);
    vector<Card> moving(from->cards.begin() + index, from->cards.end());
    save.number = moving.size();
    switch (where->type) {
        case 2:
            if (moving.size() != 1)
                return 0;
            if (where->cards.size() && moving[0].type == 14)
                return 0;

            if (where->cards.size())
            {
                if (where->cards.back().color != moving[0].color ||
                    where->cards.back().type != moving[0].type + 1)
                    return 0;
            }
            break;

        case 3:
            if ((where->cards.size() && moving[0].type == 13))
                return 0;

            if (where->cards.size())
            {
                if ((where->cards.back().color + moving[0].color) % 2 ||
                    where->cards.back().type != moving[0].type + 1)
                    return 0;
            }
            break;
        default:
            return 0;
    }
    from->cards.erase(from->cards.begin() + index, from->cards.end());
    where->add(moving);
    if (from->cards.size() && !from->cards.back().visible)
    {
        from->cards.back().visible = true;
        save.turned = true;
    }

    for (auto &pile: this->bottomPiles)
    {
        if (pile.cards.size())
            break;
        return 2;
    }

    this->moves.push_back(save);

    return 1;
}

void Game::draw()
{
    if (this->pickPile.cards.size())
    {
        Card tmp = this->pickPile.cards.back();
        tmp.visible = true;
        this->dropPile.cards.push_back(tmp);
        this->pickPile.cards.pop_back();
    }
    else
    {
        if (!this->dropPile.cards.size())
            return;

        for (auto card = this->dropPile.cards.rbegin();
             card != this->dropPile.cards.rend(); ++card)
        {
            card->visible = false;
            this->pickPile.cards.push_back(*card);
        }
        this->dropPile.cards.erase(this->dropPile.cards.begin(),
                                   this->dropPile.cards.end());
    }
}

void Game::undo()
{
    if (!this->moves.size())
        return

    
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
    random_shuffle(pile.begin(), pile.end());
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
