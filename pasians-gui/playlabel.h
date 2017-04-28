#ifndef PLAYLABEL_H
#define PLAYLABEL_H

#include <QLabel>
#include <QGraphicsProxyWidget>
#include <QMouseEvent>
#include <QPoint>

class PlayLabel;

#include "pasians.h"
#include "ggame.h"

class PlayLabel : public QLabel
{
    Q_OBJECT

public:
    PlayLabel();
    void setWrapper(QGraphicsProxyWidget *wrapper);

    void setPos(QPoint &point);
    void setPile(Pile *pile);
    void changePile(Pile *pile);
    void setCard(Card &card, Pile &pile, GGame *ggame);
    void setZ(int z);

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void moveCard(QPoint &position);

private:
    QGraphicsProxyWidget *cardWrapper = NULL;
    Pile *actualPile = NULL;
    Card *gameCard = NULL;

    GGame *game = NULL;

    vector<Card*> childs;
};

#endif // PLAYLABEL_H
