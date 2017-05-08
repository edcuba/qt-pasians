/**
* playlabel.cpp
* Pasians GUI - ICP 2016/2017
* Authors:
*   Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
*   Ondrej Kurak <xkurak00@stud.fit.vutbr.cz>
**/

#include "playlabel.h"
#include "cards.h"
#include <QMouseEvent>
#include <QPointF>
#include <algorithm>
#include <QSize>
#include <QPixmap>

#define DEBUGMODE false

#include <iostream>


/**
* @brief create holder for card
**/
PlayLabel::PlayLabel(Card *card, QSize &cardSize)
{
    card->parent = this;
    gameCard = card;

    cardVisible = card->visible;

    updateImage();
    setFixedSize(cardSize);

    setAttribute(Qt::WA_TranslucentBackground);
}

/**
* @brief update card image
**/
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


/**
* @brief show card content
**/
void PlayLabel::reveal()
{
    if (!cardVisible) {
        gameCard->visible = true;
        cardVisible = true;
        updateImage();
    }
}

/**
* @brief hide card content
**/
void PlayLabel::hide()
{
    if (cardVisible) {
        gameCard->visible = false;
        cardVisible = false;
        updateImage();
    }
}

/**
* @handle mouse click on card
**/
void PlayLabel::mousePressEvent(QMouseEvent *event)
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
            for (Card *card: game->pickPile.cards) {
                PlayLabel *l = static_cast<PlayLabel *>(card->parent);
                l->actualPile = &game->pickPile;
                l->hide();
            }
        } else {
            reveal();
            actualPile = &game->dropPile;
        }
        drawing = true;
        game->redraw();
        return;
    }

    if (!cardVisible) {
        return;
    }

    unsigned index = 0;
    vector<Card *>& thisPile = actualPile->cards;
    for (Card *card: thisPile) {
        if (card->parent == this) {
            break;
        }
        index++;
    }

    for (unsigned i = index; i < thisPile.size(); ++i) {
        childs.push_back(thisPile[i]);
    }

    mouseMoveEvent(event);
}


/**
* @handle set Z coordinate of card - send it to back or front
**/
void PlayLabel::setZ(int z)
{
    cardWrapper->setZValue(z);
}


/**
* @brief handle card release
**/
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


/**
* @brief card move
**/
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

/**
* @brief set card context - its game and actual pile
**/
void PlayLabel::setContext(Pile *pile, GGame *ggame)
{
    actualPile = pile;
    game = ggame;
}

/**
* @brief temporarily move card to coordinates
**/
void PlayLabel::moveCard(QPoint &position)
{
    cardWrapper->setPos(position);
}

/**
* @brief set wrapper object for card
**/
void PlayLabel::setWrapper(PlayWrapper *wrapper)
{
    cardWrapper = wrapper;
}

/**
* @brief move card to coordinates
**/
void PlayLabel::setPos(QPoint &point)
{
    if (cardWrapper) {
        cardWrapper->setPos(point);
    }
}

/**
* @brief set actual pile for card
**/
void PlayLabel::setPile(Pile *pile)
{
    actualPile = pile;
}

/**
* @brief move card to the another pile
* @param pile target pile:
**/
void PlayLabel::changePile(Pile *pile)
{
    vector<Card *>& thisPile = actualPile->cards;

    unsigned index = 0;

    for (Card *card: thisPile) {
        if ((PlayLabel *)card->parent == this) {
            break;
        }
        index++;
    }

    switch (game->move(actualPile, pile, index)) {
    case 1:
        break;
    case 2:
        game->finish();
        break;
    default:
        return;
    }

    actualPile = pile;

    for (Card *card: actualPile->cards) {
        PlayLabel *l = static_cast<PlayLabel *>(card->parent);
        l->actualPile = pile;
    }
}


/**
* @brief crate card hash in format type-color
**/
string PlayLabel::hash()
{
    return to_string((int) gameCard->type) + "-" + to_string((int) gameCard->color);
}

/**
* @brief convert card to placeholder
**/
void PlayLabel::setPlaceHolder()
{
    placeHolder = true;
}

/**
* @brief set new cardsize
**/
void PlayLabel::setSize(QSize &size)
{
    setFixedSize(size);
    wrapper()->adjust();

}

/**
* @returns card wrapper
**/
PlayWrapper *PlayLabel::wrapper() const
{
    return cardWrapper;
}
