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
    char type;   // 2-10, J, Q, K, A
    char color;  // 0-3, hearts, clubs, diamonds, spades
    std::pair<int, int> position; // x,y
    Card& parent;

    Card();
    ~Card();
protected:
};

class Game {
public:
    void setup();
    void reset();
    Game();
    ~Game();
protected:
};

#endif
