#ifndef PASIANS_H
#define PASIANS_H

#include <QMainWindow>
#include <QRect>
#include <QSize>
#include <QGraphicsScene>
#include <QLabel>
#include <QGraphicsView>
#include <QPoint>
#include <QString>

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

    void on_actionHint_triggered();

    void on_actionHint_2_triggered();

    void on_actionHint_3_triggered();

    void on_actionHint_4_triggered();

    void on_actionSave_triggered();

    void on_actionLoad_triggered();

    void on_actionSave_2_triggered();

    void on_actionLoad_2_triggered();

    void on_actionSave_3_triggered();

    void on_actionLoad_3_triggered();

    void on_actionSave_4_triggered();

    void on_actionLoad_4_triggered();

protected:
    vector<GGame*> games;

    QGraphicsScene *scene;
    QGraphicsView *view;

    QString loadFile();
    QString saveFile();

    Ui::Pasians *ui;

    void showGames();
    GGame *generateGame();
    void showGame(GGame *game, Layout &layout);
    PlayLabel *drawCard(Card *card, QSize &cardSize);
};

#endif // PASIANS_H
