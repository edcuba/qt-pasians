#ifndef PLAYLABEL_H
#define PLAYLABEL_H

#include "ggame.h"
#include <QLabel>
#include <QGraphicsProxyWidget>
#include <QMouseEvent>
#include <QPoint>

class PlayLabel;

#include "pasians.h"

class PlayLabel : public QLabel
{
    Q_OBJECT

public:
    PlayLabel();
    void setContext(QGraphicsProxyWidget *wrapper, Pasians *pasians);

    void setPos(QPoint &point);
    void setPile(Pile *pile);
    void changePile(Pile *pile);
    void setCard(Card &card, Pile &pile);
    void setZ(int z);

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void moveCard(QPoint &position);

private:
    QGraphicsProxyWidget *cardWrapper = NULL;
    Pasians *gameWindow = NULL;
    Pile *actualPile = NULL;
    Card *gameCard = NULL;

    vector<Card*> childs;
};

#endif // PLAYLABEL_H
