#include "ggame.h"

#include <iostream>

GGame::GGame(Pasians *pasians)
{
    gameWindow = pasians;
}

void GGame::redraw()
{
    gameWindow->redraw();
}

void GGame::start()
{
    isInitialized = true;
}

bool GGame::initialized() const
{
    return isInitialized;
}

QRectF GGame::getPileBoundaries(Pile &pile)
{
    QRectF area(0, 0, 0, 0);
    if (!pile.cards.empty()) {
        Card &first = pile.cards.front();
        PlayLabel *l = static_cast<PlayLabel *>(first.parent);
        QRectF geo = l->geometry();
        area.setTop(geo.top());
        area.setLeft(geo.left());

        Card &last = pile.cards.back();
        l = static_cast<PlayLabel *>(last.parent);
        geo = l->geometry();
        area.setBottom(geo.bottom());
        area.setRight(geo.right());
    }
    return area;
}

Pile *GGame::pileAt(const QPointF &point)
{
    for (auto &pile: bottomPiles){
        QRectF p = getPileBoundaries(pile);
        if (p.contains(point)) {
            return &pile;
        }
    }

    for (auto &pile: topPiles){
        QRectF p = getPileBoundaries(pile);
        if (p.contains(point)) {
            return &pile;
        }
    }

    return NULL;
}

