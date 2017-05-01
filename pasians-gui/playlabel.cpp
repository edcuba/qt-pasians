#include "playlabel.h"
#include "cards.h"
#include <QMouseEvent>
#include <QGraphicsProxyWidget>
#include <QPointF>
#include <algorithm>
#include <QSize>
#include <QPixmap>

#include <iostream>

PlayLabel::PlayLabel(Card &card, QSize &cardSize)
{
    card.parent = this;
    gameCard = &card;

    updateImage();
    setFixedSize(cardSize);
}

void PlayLabel::updateImage()
{
    string type = "none";

    if (gameCard->visible){
        type = hash();
    }

    string path = cardImg.find(type)->second;

    QPixmap img(path.c_str());

    setPixmap(img);

    setScaledContents(true);
}

void PlayLabel::reveal()
{
    if (!cardVisible) {
        gameCard->visible = true;
        cardVisible = true;
        updateImage();
    }
}

void PlayLabel::mousePressEvent(QMouseEvent * event)
{
    childs.clear();

    QSize s = size();
    QPointF p = cardWrapper->pos();

    p.setX(p.x() + s.width() / 2);
    p.setY(p.y() + s.height() / 2);

    Pile *pile = game->pileAt(p, NULL);

    if (pile->type == 0) {
        game->draw();
        reveal();
        game->redraw();
        return;
    }

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
    Pile *pile = game->pileAt(p, actualPile);
    if (pile && pile->type > 1) {
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

void PlayLabel::setContext(Pile &pile, GGame *ggame)
{
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
    vector<Card>& thisPile = actualPile->cards;

    unsigned index = 0;

    for (Card &card: thisPile) {
        if ((PlayLabel *)card.parent == this) {
            break;
        }
        index++;
    }

    switch (game->move(actualPile, pile, index)) {
    case 1:
    case 2:
        break;
    default:
        return;
    }

    actualPile = pile;

    for (auto &card: actualPile->cards) {
        PlayLabel *l = static_cast<PlayLabel *>(card.parent);
        l->actualPile = pile;
        l->gameCard = &card;
    }


}

string PlayLabel::hash()
{
    return to_string((int) gameCard->type) + "-" + to_string((int) gameCard->color);
}

