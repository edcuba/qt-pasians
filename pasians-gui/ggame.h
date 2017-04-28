#ifndef GGAME_H
#define GGAME_H

#include <QPointF>
#include <QRectF>

class GGame;

#include "../src/paslib.h"
#include "pasians.h"
#include "playlabel.h"

class GGame : public Game
{
public:
    GGame(Pasians *pasians);
    void start();
    void redraw();
    bool initialized() const;
    Pile *pileAt(const QPointF &point);
    QRectF getPileBoundaries(Pile &pile);
private:
    bool isInitialized = false;
    Pasians *gameWindow = NULL;
};

#endif // GGAME_H
