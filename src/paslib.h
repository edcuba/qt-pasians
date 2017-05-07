/**
* paslib.h
* Pasians library header - ICP 2016/2017
* Authors:
*   Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
*   Ondrej Kurak <xkurak00@stud.fit.vutbr.cz>
**/

#ifndef _PASLIB_H
#define _PASLIB_H

#include <utility>
#include <vector>
#include "json/json.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>


using namespace std;

class Card {
public:
    char type {0};   // 2-10, J (11), Q (12), K (13), A (1)
    char color {4};  // 0-3, hearts, clubs, diamonds, spades
    bool visible {false};
    Card(char _type, char _color);
    Card(string str);
    void *parent;
};

class Heap {
public:
    void add(vector<Card *> &_cards);
    void add(Card *card);
    void add(Heap &heap);
    Heap take(int count);
    void showTop();
    vector<Card *> cards;
};

class Pile : public Heap {
public:
    int type; // 0-> pickPile , 1->dropPile,  2->toppiles, 3->bottomPiles
    void setPlaceHolder(void *placeHolder);
    void *placeHolder() const;

protected:
    void *_placeHolder = NULL;
};

class Move
{
public:
    Pile *from;
    Pile *where;
    int number = 0;
    bool turned = false;
    Move(Pile *_from, Pile *_where);

};

class Game {
public:
    void setup();
    void reset();
    void load(string);
    void save(string);

    int move(Pile *from, Pile *where, int index);
    void draw();
    void undo();
    Move hint();
    vector<Pile> bottomPiles{7};
    vector<Pile> topPiles{4};
    Pile pickPile;
    Pile dropPile;
    vector<Move> moves;

protected:
    void generateDeck();
    vector<Card *> getCards();
};

#endif //_PASLIB_H
