#ifndef PASIANS_H
#define PASIANS_H

#include <QMainWindow>
#include <QRect>
#include <QSize>
#include <QGraphicsScene>
#include <QLabel>
#include <QGraphicsView>
#include <QPoint>

const int cardOffsetMod = 6;

namespace Ui {
    class Pasians;
}

class Pasians;

#include "playlabel.h"
#include "layout.h"
#include "ggame.h"

class Pasians : public QMainWindow
{
    Q_OBJECT

public:
    explicit Pasians(QWidget *parent = 0);
    void resizeEvent(QResizeEvent* event);
    void finalizeGame(GGame *game);
    ~Pasians();

    void redraw();
private slots:
    void on_actionNew_triggered();

    void on_actionQuit_4_triggered();

    void on_actionQuit_3_triggered();

    void on_actionQuit_2_triggered();

    void on_actionQuit_triggered();

    void on_actionUndo_triggered();

    void on_actionUndo_2_triggered();

    void on_actionUndo_3_triggered();

    void on_actionUndo_4_triggered();

protected:
    vector<GGame*> games;

    QGraphicsScene *scene;
    QGraphicsView *view;

    Ui::Pasians *ui;

    void showGames();
    GGame *generateGame();
    void showGame(GGame *game, Layout &layout);
    PlayLabel *drawCard(Card *card, QSize &cardSize);
};

#endif // PASIANS_H
