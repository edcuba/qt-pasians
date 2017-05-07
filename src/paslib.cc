/**
* paslib.cc
* Pasians library - ICP 2016/2017
* Authors:
*   Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
*   Ondrej Kurak <xkurak00@stud.fit.vutbr.cz>
**/

#include "paslib.h"

using namespace std;

Card::Card(char _type, char _color)
{
    type = _type;
    color = _color;
}

Card::Card(string str)
{
    istringstream iss(str);
    vector<string> tokens{istream_iterator<string>{iss},
                          istream_iterator<string>{}};
    type = stoi(tokens[0]);
    color = stoi(tokens[1]);
    visible = stoi(tokens[2]) ? true : false;
}


void Game::setup()
{
    vector<Card *> cards = getCards();
    int i = 0;
    for (auto &card: cards)
    {
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
    Move save(from, where);
    vector<Card *> moving(from->cards.begin() + index, from->cards.end());
    save.number = moving.size();
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
        for (size_t a = 0; a < bottomPiles[i].cards.size(); ++a)
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

void Game::load(string file_path)
{
    Json::Value yolo;

    ifstream in(file_path);
    in >> yolo;
    in.close();

    dropPile.cards.clear();
    pickPile.cards.clear();

    for (auto &pile: bottomPiles) {
        pile.cards.clear();
    }

    for (auto &pile: topPiles) {
        pile.cards.clear();
    }

    moves.clear();

    for (auto str: yolo["pickPile"])
    {
        Card *card = new Card(str.asString());
        pickPile.cards.push_back(card);
    }
    for (auto str: yolo["dropPile"])
    {
        Card *card = new Card(str.asString());
        dropPile.cards.push_back(card);
    }

    for (size_t i = 0; i < topPiles.size(); ++i)
    {
        string name = "topPile-" + to_string(i);
        for (auto str: yolo[name])
        {
            Card *card = new Card(str.asString());
            topPiles[i].cards.push_back(card);
        }
    }

    for (size_t i = 0; i < bottomPiles.size(); ++i)
    {
        string name = "bottomPile-" + to_string(i);
        for (auto str: yolo[name])
        {
            Card *card = new Card(str.asString());
            bottomPiles[i].cards.push_back(card);
        }
    }

    for (auto str: yolo["moves"])
    {
        Pile *from;
        Pile *where;
        string tmp = str.asString();
        istringstream iss(tmp);
        vector<string> tokens{istream_iterator<string>{iss},
                              istream_iterator<string>{}};
        int num = stoi(tokens[1]);
        if (tokens[0] == "b")
        {
            from = &bottomPiles[num];
        }
        else if (tokens[0] == "t")
        {
            from = &topPiles[num];
        }
        else if (tokens[0] == "d")
        {
            from = &dropPile;
        }
        else if (tokens[0] == "p")
        {
            from = &pickPile;
        }
        
        num = stoi(tokens[3]);
        if (tokens[2] == "b")
        {
            where = &bottomPiles[num];
        }
        else if (tokens[2] == "t")
        {
            where = &topPiles[num];
        }
        else if (tokens[2] == "d")
        {
            where = &dropPile;
        }
        else if (tokens[2] == "p")
        {
            where = &pickPile;
        }
        Move save(from, where);
        save.number = stoi(tokens[4]);
        moves.push_back(save);
    }
}

void Game::save(string file_path)
{
    Json::Value yolo;

    Json::Value ppile;
    for (auto &card: pickPile.cards) {
        ppile.append(to_string(card->type) + " " +to_string(card->color) + " " +(card->visible ? "1" : "0"));
    }
    yolo["pickPile"] = ppile;

    Json::Value dpile;
    for (auto &card: dropPile.cards) {
        dpile.append(to_string(card->type) + " " +to_string(card->color) + " " +(card->visible ? "1" : "0"));
    }
    yolo["dropPile"] = dpile;

    for (size_t i = 0; i < topPiles.size(); ++i)
    {
        Json::Value topPile;
        string name = "topPile-" + to_string(i);
        for (auto &card: topPiles[i].cards) {
            topPile.append(to_string(card->type) + " " +to_string(card->color) + " " +(card->visible ? "1" : "0"));
        }
        yolo[name] = topPile;
    }
    for (size_t i = 0; i < bottomPiles.size(); ++i)
    {
        Json::Value bottomPile;
        string name = "bottomPile-" + to_string(i);
        for (auto &card: bottomPiles[i].cards) {
            bottomPile.append(to_string(card->type) + " " +to_string(card->color) + " " +(card->visible ? "1" : "0"));
        }
        yolo[name] = bottomPile;
    }

    Json::Value j_moves;
    for (auto &move: moves)
    {
        string str = "";
        if (move.from == &bottomPiles[0])
            str += "b 0";
        else if (move.from == &bottomPiles[1])
            str += "b 1";
        else if (move.from == &bottomPiles[2])
            str += "b 2";
        else if (move.from == &bottomPiles[3])
            str += "b 3";
        else if (move.from == &bottomPiles[4])
            str += "b 4";
        else if (move.from == &bottomPiles[5])
            str += "b 5";
        else if (move.from == &bottomPiles[6])
            str += "b 6";
        else if (move.from == &topPiles[0])
            str += "t 0";
        else if (move.from == &topPiles[1])
            str += "t 1";
        else if (move.from == &topPiles[2])
            str += "t 2";
        else if (move.from == &topPiles[3])
            str += "t 3";
        else if (move.from == &dropPile)
            str += "d 0";
        else if (move.from == &pickPile)
            str += "p 0";

        str += " ";
        if (move.where == &bottomPiles[0])
            str += "b 0";
        else if (move.where == &bottomPiles[1])
            str += "b 1";
        else if (move.where == &bottomPiles[2])
            str += "b 2";
        else if (move.where == &bottomPiles[3])
            str += "b 3";
        else if (move.where == &bottomPiles[4])
            str += "b 4";
        else if (move.where == &bottomPiles[5])
            str += "b 5";
        else if (move.where == &bottomPiles[6])
            str += "b 6";
        else if (move.where == &topPiles[0])
            str += "t 0";
        else if (move.where == &topPiles[1])
            str += "t 1";
        else if (move.where == &topPiles[2])
            str += "t 2";
        else if (move.where == &topPiles[3])
            str += "t 3";
        else if (move.where == &dropPile)
            str += "d 0";
        else if (move.where == &pickPile)
            str += "p 0";

        str += " " + to_string(move.number);
        j_moves.append(str);
    }
    yolo["moves"] = j_moves;

    ofstream out(file_path);
    out << yolo;
    out.close();
}

void Pile::setPlaceHolder(void *placeHolder)
{
    _placeHolder = placeHolder;
}

void *Pile::placeHolder() const
{
    return _placeHolder;
}
