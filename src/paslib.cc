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
        cout << (int) card.type << "-" << (int) card.color << endl;
        if (i < 28)
        {

            if (i < 1)
                bottomPiles[0].add(card);
            else if (i < 3)
                bottomPiles[1].add(card);
            else if (i < 6)
                bottomPiles[2].add(card);
            else if (i < 10)
                bottomPiles[3].add(card);
            else if (i < 15)
                bottomPiles[4].add(card);
            else if (i < 21)
                bottomPiles[5].add(card);
            else
                bottomPiles[6].add(card);
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
    cout << "in" << endl;
    Move save(from, where);
    vector<Card> moving(from->cards.begin() + index, from->cards.end());
    save.number = moving.size();
    switch (where->type) {
        case 2:
            cout << "case 2" << endl;
            if (moving.size() != 1)
                return 0;
            if (/*(!where->cards.empty() && moving[0].type == 1) ||*/
                (where->cards.empty() && moving[0].type != 1))
            {
                cout << "case A" << endl;
                return 0;
            }

            if (!where->cards.empty())
            {
                if (where->cards.back().color != moving[0].color ||
                    where->cards.back().type - moving[0].type != -1)
                {
                    cout << "case B" << endl;
                    return 0;
                }
            }
            break;

        case 3:
        cout << "case 3" << endl;

            if ((!where->cards.empty() && moving[0].type == 13)) {
                cout << "case A" << endl;
                return 0;
            }

            if (!where->cards.empty())
            {
                if ((where->cards.back().color + moving[0].color) % 2 == 0 ||
                    where->cards.back().type != moving[0].type + 1) {
                    cout << "Karta " << (int)moving[0].type << endl;
                    cout << (int) where->cards.back().color << " " << (int)moving[0].color << endl;
                    cout << "case B" << endl;
                    return 0;
                }
            }
            break;
        default:
            cout << "case C" << endl;
            return 0;
    }
    from->cards.erase(from->cards.begin() + index, from->cards.end());
    where->add(moving);
    if (from->cards.size() && !from->cards.back().visible)
    {
        from->cards.back().visible = true;
        save.turned = true;
    }

    for (auto &pile: bottomPiles)
    {
        if (pile.cards.size())
            break;
        return 2;
    }

    moves.push_back(save);

    return 1;
}

void Game::draw()
{
    if (pickPile.cards.size())
    {
        Card tmp = pickPile.cards.back();
        tmp.visible = true;
        dropPile.cards.push_back(tmp);
        pickPile.cards.pop_back();

        Move save(&(pickPile), &(dropPile));
        moves.push_back(save);
    }
    else
    {
        if (!dropPile.cards.size())
            return;


        for (auto card = dropPile.cards.rbegin();
             card != dropPile.cards.rend(); ++card)
        {
            card->visible = false;
            pickPile.cards.push_back(*card);
        }
        dropPile.cards.erase(dropPile.cards.begin(),
                                   dropPile.cards.end());

       Move save(&(dropPile), &(pickPile));
       moves.push_back(save);
    }
}

void Game::undo()
{
    if (!moves.size())
        return;

    Move last = moves.back();

    if (last.from  == &(pickPile))
    {
        Card card = dropPile.cards.back();
        card.visible = false;
        pickPile.add(card);
        dropPile.cards.pop_back();
    } else if (last.from  == &(dropPile) &&
        last.where == &(pickPile))
    {
        for (auto card = pickPile.cards.rbegin();
             card != pickPile.cards.rend(); ++card)
        {
            card->visible = true;
            dropPile.cards.push_back(*card);
        }

        pickPile.cards.erase(pickPile.cards.begin(),
                             pickPile.cards.end());
        return;
    }

    moves.pop_back();

    /*
    if (last.turned)
        last.from->cards.back().visible = false;
        */


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
        Card card(t % 13 + 1, c);
        pile.push_back(card);
        c++;
        c %= 4;
    }
    srand(time(0));
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

void Game::load()
{
    Json::Value yolo;
    yolo["pasians"].append("Game1");

    cout << yolo << endl;
}


void Pile::setPlaceHolder(void *placeHolder)
{
    _placeHolder = placeHolder;
}

void *Pile::placeHolder() const
{
    return _placeHolder;
}
