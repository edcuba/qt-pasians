#ifndef GGAME_H
#define GGAME_H

#include <QPointF>
#include <QRectF>
#include <QGraphicsScene>

class GGame;

#include "../src/paslib.h"
#include "pasians.h"
#include "playlabel.h"
#include "layout.h"

class GGame : public Game
{
public:
    GGame(Pasians *pasians);
    void start();
    void redraw();
    bool initialized() const;
    Pile *pileAt(const QPointF &point, Pile *ignore);
    QRectF getPileBoundaries(Pile &pile);
    void setupPlaceHolders(Layout &layout, QGraphicsScene *scene);
    void finish();
    bool done() const;
    void performUndo();
    void performHint();
protected:
    void regenerate();
    bool isInitialized = false;
    bool isDone = false;
    Pasians *gameWindow = NULL;
};

#endif // GGAME_H
