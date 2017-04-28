#include "playlabel.h"
#include <QMouseEvent>
#include <QGraphicsProxyWidget>
#include <QPointF>
#include <algorithm>
#include <QSize>

#include <iostream>

PlayLabel::PlayLabel() {}

void PlayLabel::mousePressEvent(QMouseEvent * event)
{
    childs.clear();

    unsigned index = 0;
    vector<Card>& thisPile = actualPile->cards;
    for (Card &card: thisPile) {
        if ((PlayLabel *)card.parent == this) {
            break;
        }
        index++;
    }

    for (unsigned i = index; i < thisPile.size(); ++i) {
        childs.push_back(&thisPile[i]);
    }

    mouseMoveEvent(event);
}

void PlayLabel::setZ(int z)
{
    cardWrapper->setZValue(z);
}


void PlayLabel::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    QPointF p = cardWrapper->pos();
    QSize s = size();
    p.setX(p.x() + s.width() / 2);
    p.setY(p.y() + s.height() / 2);
    Pile *pile = game->pileAt(p);
    if (pile) {
        for (auto &card: childs) {
            PlayLabel *l = static_cast<PlayLabel *>(card->parent);
            l->changePile(pile);
        }
        changePile(pile);
    }
    game->redraw();

}

void PlayLabel::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos;
    int height = size().height();
    int z = 100;
    pos.setX(x() + event->x() - size().width() / 2);
    pos.setY(y() + event->y() - height / 2);
    setZ(z);
    moveCard(pos);
    if (!childs.empty()) {
        for (Card *child: childs) {
            z++;
            pos.setY(pos.y() + height / cardOffsetMod);
            PlayLabel *l = (PlayLabel *) child->parent;
            l->moveCard(pos);
            l->setZ(z);
        }
    }
}

void PlayLabel::setCard(Card &card, Pile &pile, GGame *ggame)
{
    card.parent = this;

    gameCard = &card;
    actualPile = &pile;

    game = ggame;
}

void PlayLabel::moveCard(QPoint &position)
{
    cardWrapper->setPos(position);
}

void PlayLabel::setWrapper(QGraphicsProxyWidget *wrapper)
{
    cardWrapper = wrapper;
}

void PlayLabel::setPos(QPoint &point)
{
    if (cardWrapper) {
        cardWrapper->setPos(point);
    }
}

void PlayLabel::setPile(Pile *pile)
{
    actualPile = pile;
}

void PlayLabel::changePile(Pile *pile)
{
    unsigned index = 0;
    vector<Card>& thisPile = actualPile->cards;
    for (Card &card: thisPile) {
        if ((PlayLabel *)card.parent == this) {
            break;
        }
        index++;
    }
    Card c = thisPile[index];
    thisPile.erase(thisPile.begin() + index);
    actualPile = pile;
    actualPile->add(c);
}

