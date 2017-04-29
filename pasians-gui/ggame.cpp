#include "ggame.h"

#include <iostream>

GGame::GGame(Pasians *pasians)
{
    gameWindow = pasians;
}

void GGame::redraw()
{
    for (Pile &pile: bottomPiles) {
        if (pile.cards.empty()) {
            continue;
        }
        Card  &c = pile.cards.back();
        if (!c.visible) {
            PlayLabel *l = static_cast<PlayLabel *>(c.parent);
            l->reveal();
        }
    }
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

Pile *GGame::pileAt(const QPointF &point, Pile *ignore)
{
    QRectF p = getPileBoundaries(pickPile);

    if (ignore != &pickPile && p.contains(point)) {
        return &pickPile;
    }

    p = getPileBoundaries(dropPile);

    if (ignore != &dropPile && p.contains(point)) {
        return &dropPile;
    }

    for (auto &pile: bottomPiles){
        p = getPileBoundaries(pile);
        if (ignore != &pile && p.contains(point)) {
            return &pile;
        }
    }

    for (auto &pile: topPiles){
        p = getPileBoundaries(pile);
        if (ignore != &pile && p.contains(point)) {
            return &pile;
        }
    }

    return NULL;
}

