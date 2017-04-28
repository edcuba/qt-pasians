#include "playlabel.h"
#include <QMouseEvent>
#include <QGraphicsProxyWidget>
#include <QPoint>
#include <algorithm>

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


void PlayLabel::mouseReleaseEvent(QMouseEvent * event)
{
    Q_UNUSED(event);
    gameWindow->redraw();
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

void PlayLabel::setCard(Card &card, Pile &pile)
{
    card.parent = this;

    gameCard = &card;
    actualPile = &pile;
}

void PlayLabel::moveCard(QPoint &position)
{
    cardWrapper->setPos(position);
}

void PlayLabel::setContext(QGraphicsProxyWidget *wrapper, Pasians *pasians)
{
    cardWrapper = wrapper;
    gameWindow = pasians;
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
    Q_UNUSED(pile);
}

