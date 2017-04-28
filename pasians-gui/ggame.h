#ifndef GGAME_H
#define GGAME_H

#include "../src/paslib.h"

class GGame : public Game
{
public:
    void start();
    bool initialized() const;
private:
    bool isInitialized = false;
};

#endif // GGAME_H
