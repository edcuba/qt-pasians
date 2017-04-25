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

using namespace std;

class Card {
public:
    char type {0};   // 2-10, J (11), Q (12), K (13), A (14)
    char color {4};  // 0-3, hearts, clubs, diamonds, spades
    bool visible {false};
    Card *parent {nullptr};

    bool move(int x, int y);

    Card(char _type, char _color);

protected:
    pair<int, int> position {0, 0}; // x,y
};

class Heap {
public:
    void add(vector<Card> _cards);
    void add(Card card);
    void add(Heap &heap);
    Heap take(int count);
    void showTop();
    vector<Card> cards;
};

class Pile : public Heap {
public:
    int type;
    pair<int, int> position {0, 0}; // x,y
};

class Game {
public:
    void setup();
    void reset();
    void load();

    vector<Pile> bottomPiles{7};
    vector<Pile> topPiles{4};
    Pile pickPile;
    Pile dropPile;

protected:
    void generateDeck();
    vector<Card> getCards();
};

#endif
