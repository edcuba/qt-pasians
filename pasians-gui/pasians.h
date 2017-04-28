#ifndef PASIANS_H
#define PASIANS_H

#include <QMainWindow>
#include <QRect>
#include <QSize>
#include <QGraphicsScene>
#include <QLabel>
#include <QGraphicsView>
#include <QPoint>

const int cardOffsetMod = 5;

namespace Ui {
    class Pasians;
}

class Pasians;

#include "playlabel.h"
#include "layout.h"
#include "cards.h"
#include "ggame.h"

class Pasians : public QMainWindow
{
    Q_OBJECT

public:
    explicit Pasians(QWidget *parent = 0);
    void resizeEvent(QResizeEvent* event);
    ~Pasians();

    void redraw();
private:
    vector<GGame*> games;

    QGraphicsScene *scene;
    QGraphicsView *view;

    Ui::Pasians *ui;

    void showGames(Layout &layout);
    GGame *generateGame();
    void showGame(GGame *game, Layout &layout);
    PlayLabel *drawCard(Card &card, QSize &cardSize);
    string hashCard(Card &card);

    Layout activeLayout;

};

#endif // PASIANS_H
