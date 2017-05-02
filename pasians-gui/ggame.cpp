#include "ggame.h"

#include <iostream>
#include <QGraphicsProxyWidget>

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
        PlayLabel *l = static_cast<PlayLabel *>(c.parent);
        l->reveal();
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
    } else {
        PlayLabel *l = static_cast<PlayLabel *>(pile.placeHolder());
        if (l) {
            return l->geometry();
        }
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

static PlayLabel *setUpWrapper(QSize &size, QPoint &place, QGraphicsScene *scene)
{
    Card *hold = new Card(0, 0);
    hold->visible = true;
    PlayLabel *l = new PlayLabel(hold, size);
    l->setPlaceHolder();
    QGraphicsProxyWidget *w = scene->addWidget(l);
    l->setWrapper(w);
    l->setPos(place);
    return l;
}


void GGame::setupPlaceHolders(Layout &layout, QGraphicsScene *scene)
{
    QSize &size = layout.cardSize;

    PlayLabel *pick = setUpWrapper(size, layout.pick, scene);
    pick->setContext(&pickPile, this);
    pickPile.setPlaceHolder(pick);

    PlayLabel *drop = setUpWrapper(size, layout.drop, scene);
    drop->setContext(&dropPile, this);
    dropPile.setPlaceHolder(drop);

    QPoint top = layout.top;
    for (auto &pile: topPiles) {
        PlayLabel *l = setUpWrapper(size, top, scene);
        l->setContext(&pile, this);
        pile.setPlaceHolder(l);
        top.setX(top.x() + layout.cardWidth + layout.wspace);
    }

    QPoint bot = layout.bot;
    for (auto &pile: bottomPiles) {
        PlayLabel *l = setUpWrapper(size, bot, scene);
        l->setContext(&pile, this);
        pile.setPlaceHolder(l);
        bot.setX(bot.x() + layout.cardWidth + layout.wspace);
    }
}

