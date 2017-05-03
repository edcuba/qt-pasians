#include "playlabel.h"
#include "cards.h"
#include <QMouseEvent>
#include <QPointF>
#include <algorithm>
#include <QSize>
#include <QPixmap>

#define DEBUGMODE true

#include <iostream>

PlayLabel::PlayLabel(Card *card, QSize &cardSize)
{
    card->parent = this;
    gameCard = card;

    cardVisible = card->visible;

    updateImage();
    setFixedSize(cardSize);

    setAttribute(Qt::WA_TranslucentBackground);
}

void PlayLabel::updateImage()
{
    string type = "none";

    if (cardVisible){
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

void PlayLabel::hide()
{
    if (cardVisible) {
        gameCard->visible = false;
        cardVisible = false;
        updateImage();
    }
}

void PlayLabel::mousePressEvent(QMouseEvent * event)
{
    childs.clear();
    drawing = false;

    QSize s = size();
    QPointF p = cardWrapper->pos();

    p.setX(p.x() + s.width() / 2);
    p.setY(p.y() + s.height() / 2);

    Pile *pile = game->pileAt(p, NULL);

    if (!pile) {
        return;
    }

    if (pile->type == 0) {
        game->draw();
        if (placeHolder) {
            for (Card &card: game->pickPile.cards) {
                PlayLabel *l = static_cast<PlayLabel *>(card.parent);
                l->actualPile = &game->pickPile;
                l->gameCard = &card;
                l->hide();
            }
        } else {
            reveal();
            actualPile = &game->dropPile;
            gameCard = &game->dropPile.cards.back();
        }
        drawing = true;
        game->redraw();
        return;
    }

    if (!cardVisible) {
        return;
    }

    unsigned index = 0;
    vector<Card>& thisPile = actualPile->cards;
    for (Card &card: thisPile) {
        if (card.parent == this) {
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

    if (placeHolder) {
        return;
    }

    if (!cardVisible || drawing) {
        return;
    }

    if (DEBUGMODE) {
        cout << "Moving " << hash() << endl;
    }

    QPointF p = cardWrapper->pos();
    QSize s = size();
    p.setX(p.x() + s.width() / 2);
    p.setY(p.y() + s.height() / 2);
    Pile *pile = game->pileAt(p, actualPile);

    if (pile && pile->type > 1) {
        if (DEBUGMODE) {
            cout << "Target pile type " << pile->type << endl;
        }
        changePile(pile);
    } else if (DEBUGMODE) {
        cout << "No pile" << endl;
    }



    game->redraw();

}

void PlayLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (placeHolder) {
        return;
    }

    if (!cardVisible || drawing) {
        return;
    }

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
            PlayLabel *l = static_cast<PlayLabel *>(child->parent);
            l->moveCard(pos);
            l->setZ(z);
        }
    }
}

void PlayLabel::setContext(Pile *pile, GGame *ggame)
{
    actualPile = pile;
    game = ggame;
}

void PlayLabel::moveCard(QPoint &position)
{
    cardWrapper->setPos(position);
}

void PlayLabel::setWrapper(PlayWrapper *wrapper)
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

void PlayLabel::setPlaceHolder()
{
    placeHolder = true;
}

void PlayLabel::setSize(QSize &size)
{
    setFixedSize(size);
    wrapper()->adjust();

}

PlayWrapper *PlayLabel::wrapper() const
{
    return cardWrapper;
}


