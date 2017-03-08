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

class Card {
public:
    char type {0};   // 2-10, J, Q, K, A
    char color {0};  // 1-4, hearts, clubs, diamonds, spades
    bool visible {false};
    Card *parent {nullptr};

    bool move(int x, int y);

    Card(char _type, char _color);

protected:
    std::pair<int, int> position {0, 0}; // x,y
};

class Game {
public:
    void setup();
    void reset();
    void load();
protected:
    void generateDeck();
};

#endif
