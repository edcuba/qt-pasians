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
    PlayLabel(Card &card, QSize &cardSize);
    void setWrapper(QGraphicsProxyWidget *wrapper);
    void setPos(QPoint &point);
    void setPile(Pile *pile);
    void changePile(Pile *pile);
    void setContext(Pile &pile, GGame *ggame);
    void setZ(int z);
    string hash();
    void reveal();

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void moveCard(QPoint &position);
    void updateImage();

    bool cardVisible = false;

private:
    QGraphicsProxyWidget *cardWrapper = NULL;
    Pile *actualPile = NULL;
    Card *gameCard = NULL;

    GGame *game = NULL;

    vector<Card*> childs;
};

#endif // PLAYLABEL_H
