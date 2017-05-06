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
    vector<Card *> cards = getCards();
    int i = 0;
    for (auto &card: cards)
    {
        //cout << (int) card->type << "-" << (int) card->color << endl;
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
    std::cout << "Pile: " << (int)where->type << '\n';
cout << where << endl;
    cout << "in" << endl;
    Move save(from, where);
    vector<Card *> moving(from->cards.begin() + index, from->cards.end());
    save.number = moving.size();
    std::cout << "Pile: " << (int)where->type << '\n';
    switch (where->type) {
        case 2:
            cout << "case 2" << endl;
            if (moving.size() != 1)
                return 0;
            if (/*(!where->cards.empty() && moving[0].type == 1) ||*/
                (where->cards.empty() && moving[0]->type != 1))
            {
                cout << "case A" << endl;
                return 0;
            }

            if (!where->cards.empty())
            {
                if (where->cards.back()->color != moving[0]->color ||
                    where->cards.back()->type - moving[0]->type != -1)
                {
                    cout << "case B" << endl;
                    return 0;
                }
            }
            break;

        case 3:
        cout << "case 3" << endl;

            if (where->cards.empty() && moving[0]->type != 13)
            {
                cout << "case A" << endl;
                return 0;
            }

            if (!where->cards.empty())
            {
                if ((where->cards.back()->color + moving[0]->color) % 2 == 0 ||
                    where->cards.back()->type != moving[0]->type + 1) {
                    cout << "Karta " << (int)moving[0]->type << endl;
                    cout << (int) where->cards.back()->color << " " << (int)moving[0]->color << endl;
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
    if (!from->cards.empty() && !from->cards.back()->visible)
    {
        from->cards.back()->visible = true;
        save.turned = true;
    }

    int num = 0;
    for (int i = 0; i < 4; ++i)
        num += topPiles[i].cards.size();

    if (num == 52)
        return 2;

    moves.push_back(save);

    return 1;
}

void Game::draw()
{
    if (!pickPile.cards.empty())
    {
        Card *tmp = pickPile.cards.back();
        tmp->visible = true;
        dropPile.cards.push_back(tmp);
        pickPile.cards.pop_back();

        Move save(&(pickPile), &(dropPile));
        moves.push_back(save);
    }
    else
    {
        if (dropPile.cards.empty())
            return;

        for (auto card = dropPile.cards.rbegin();
             card != dropPile.cards.rend(); ++card)
        {
            Card *c = *card;
            c->visible = false;
            pickPile.cards.push_back(c);
        }
        dropPile.cards.erase(dropPile.cards.begin(), dropPile.cards.end());

       Move save(&(dropPile), &(pickPile));
       moves.push_back(save);
    }
}

void Game::undo()
{
    if (moves.empty())
        return;

    Move last = moves.back();

    if (last.from  == &(pickPile))
    {
        Card *card = dropPile.cards.back();
        card->visible = false;
        pickPile.add(card);
        dropPile.cards.pop_back();
        moves.pop_back();
        return;
    }

    if (last.from  == &(dropPile) && last.where == &(pickPile))
    {
        std::cout << "DAvam" << '\n';
        for (auto card = pickPile.cards.rbegin();
             card != pickPile.cards.rend(); ++card)
        {
            Card *c = *card;
            c->visible = true;
            dropPile.cards.push_back(c);
        }

        pickPile.cards.erase(pickPile.cards.begin(),
                             pickPile.cards.end());
        moves.pop_back();
        return;
    }

    int index = last.where->cards.size() - last.number;
    vector<Card *> moving(last.where->cards.begin() + index,
                          last.where->cards.end());
    if (last.turned)
    {
        last.from->cards.back()->visible = false;
    }

    last.where->cards.erase(last.where->cards.begin() + index,
                            last.where->cards.end());
    last.from->add(moving);



    moves.pop_back();
}

Move Game::hint()
{
    if (!dropPile.cards.empty())
    {
        Card *c = dropPile.cards.back();
        for (auto &pile: topPiles)
        {
            if (pile.cards.empty())
            {
                if (c->type != 1)
                    continue;
            }
            else if (c->color != pile.cards[0]->color ||
                     pile.cards.back()->type - c->type != -1)
            {
                continue;
            }
            Move save(&dropPile, &pile);
            save.number = dropPile.cards.size() - 1;
            return save;
        }
    }


    for (int i = 6; i >= 0; --i)
    {
        Card *c = NULL;
        for (auto &pile: topPiles)
        {
            if (bottomPiles[i].cards.empty())
                continue;
            c = bottomPiles[i].cards.back();
            if (pile.cards.empty())
            {
                if (c->type != 1)
                    continue;
            }
            else if (c->color != pile.cards[0]->color ||
                     pile.cards.back()->type - c->type != -1)
            {
                continue;
            }
            Move save(&bottomPiles[i], &pile);
            save.number = bottomPiles[i].cards.size() - 1;
            return save;
        }
    }

    int index = -1;
    for (int i = 6; i >= 0; --i)
    {
        Card *c = NULL;
        index = -1;
        for (int a = 0; a < bottomPiles[i].cards.size(); ++a)
        {
            if (bottomPiles[i].cards[a]->visible)
            {
                c = bottomPiles[i].cards[a];
                index = a;
                if (c->type == 13 && index == 0)
                {
                    c = NULL;
                }
                break;
            }
        }
        if (!c)
            continue;

        for (auto &pile: bottomPiles)
        {
            if (pile.cards.empty())
            {
                if (c->type != 13)
                    continue;
            }
            else if ((c->color + pile.cards.back()->color) % 2 == 0 ||
                     c->type - pile.cards.back()->type != -1)
            {
                continue;
            }

            Move save(&bottomPiles[i], &pile);
            save.number = index;
            return save;
        }
    }

    if (!dropPile.cards.empty())
    {
        Card *c = dropPile.cards.back();
        for (auto &pile: bottomPiles)
        {
            if (pile.cards.empty())
            {
                if (c->type != 13)
                    continue;
            }
            else if ((c->color + pile.cards.back()->color) % 2 == 0 ||
                      c->type - pile.cards.back()->type != -1)
            {
                continue;
            }
            Move save(&dropPile, &pile);
            save.number = dropPile.cards.size() - 1;
            return save;
        }
    }

    if (!pickPile.cards.empty() || !dropPile.cards.empty())
    {
        Move save(&pickPile, &dropPile);
        save.number = -2;
        return save;
    }

    Move save(NULL, NULL);
    save.number = -1;
    return save;
}

/**
* Generate vector of 52 play cards
* @return #vector of play cards, order is constant
**/
vector<Card *> Game::getCards()
{
    vector<Card *> pile;
    char c = 0;
    for (char t = 0; t < 52; ++t) {
        Card *card = new Card(t % 13 + 1, c);
        pile.push_back(card);
        c++;
        c %= 4;
    }
    srand(time(0));
    random_shuffle(pile.begin(), pile.end());
    return pile;
}


void Heap::add(vector<Card *> &_cards)
{
    cards.insert(cards.end(), _cards.begin(), _cards.end());
}

void Heap::add(Card *card)
{
    cards.push_back(card);
}

void Heap::showTop()
{
    if (!cards.empty()) {
        cards.back()->visible = true;
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
