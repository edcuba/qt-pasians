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
    PlayLabel(Card *card, QSize &cardSize);
    void setWrapper(QGraphicsProxyWidget *wrapper);
    void setPos(QPoint &point);
    void setPile(Pile *pile);
    void changePile(Pile *pile);
    void setContext(Pile *pile, GGame *ggame);
    void setZ(int z);
    string hash();
    void reveal();
    void hide();
    void setPlaceHolder();

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void moveCard(QPoint &position);
    void updateImage();

    bool cardVisible = false;

    bool placeHolder = false;

private:
    QGraphicsProxyWidget *cardWrapper = NULL;
    Pile *actualPile = NULL;
    Card *gameCard = NULL;

    GGame *game = NULL;

    vector<Card*> childs;

    bool drawing;
};

#endif // PLAYLABEL_H
