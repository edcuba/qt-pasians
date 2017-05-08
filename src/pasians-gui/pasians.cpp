/**
* pasians.cpp
* Pasians GUI - ICP 2016/2017
* Authors:
*   Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
*   Ondrej Kurak <xkurak00@stud.fit.vutbr.cz>
**/

#include "pasians.h"
#include "ui_pasians.h"
#include <QSize>
#include <QResizeEvent>
#include <QGraphicsScene>
#include <vector>
#include <iostream>
#include <QMessageBox>
#include "playwrapper.h"
#include <QFileDialog>

/**
* @brief game window initialization
**/
Pasians::Pasians(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Pasians)
{
    ui->setupUi(this);

    scene = new QGraphicsScene();

    view = ui->graphicsView;

    view->setScene(scene);

    GGame *game = generateGame();
    games.push_back(game);

    view->show();

    showGames();

    view->setMouseTracking(true);
    this->setMouseTracking(true);
    ui->gameBoard->setMouseTracking(true);
}

/**
* @brief handle window resize - refresh screen
**/
void Pasians::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    showGames();
}

/**
* @brief pasians destructor
**/
Pasians::~Pasians()
{
    delete ui;
    for (auto &game: games) {
        delete game;
    }
}

/**
* @brief show "You won" dialogue
**/
static void showFinish()
{
    QMessageBox msgBox;
    msgBox.setText("You won!");
    msgBox.exec();
}

/**
* @brief check if game is finished and redraw card positions
**/
void Pasians::redraw()
{
    for (GGame *game: games) {
        if (game->done()) {
            showFinish();
        }
    }
    showGames();
}

/**
 * @brief Pasians::showGames show all games
**/
void Pasians::showGames()
{
    if (games.empty()) {
        games.push_back(generateGame());
    }

    size_t gsize = games.size();

    Layout layout(size());

    ui->actionNew->setEnabled(gsize < 4);
    ui->menuGame_1->setEnabled(gsize > 0);
    ui->menuGame_2->setEnabled(gsize > 1);
    ui->menuGame_3->setEnabled(gsize > 2);
    ui->menuGame_4->setEnabled(gsize > 3);

    switch (gsize) {
    case 1:
        showGame(games.front(), layout);
        break;
    case 2:
    {
        QSize s = layout.lsize;
        s.setWidth(s.width() / 2);

        Layout split0(s, QPoint());
        showGame(games[0], split0);

        Layout split1(s, QPoint(s.width(), 0));

        showGame(games[1], split1);
        break;
    }
    case 3:
    {
        QSize s = layout.lsize;

        s.setHeight(s.height() / 2);
        s.setWidth(s.width() / 2);

        Layout split0(s, QPoint());
        Layout split1(s, QPoint(s.width(), 0));
        Layout split2(s, QPoint(0, s.height()));

        showGame(games[0], split0);
        showGame(games[1], split1);
        showGame(games[2], split2);

        break;
    }
    case 4:
    {
        QSize s = layout.lsize;

        s.setHeight(s.height() / 2);
        s.setWidth(s.width() / 2);

        Layout split0(s, QPoint());
        Layout split1(s, QPoint(s.width(), 0));
        Layout split2(s, QPoint(0, s.height()));
        Layout split3(s, QPoint(s.width(), s.height()));

        showGame(games[0], split0);
        showGame(games[1], split1);
        showGame(games[2], split2);
        showGame(games[3], split3);

        break;
    }
        break;
    default:
        return;
    }

    view->setSceneRect(scene->itemsBoundingRect());
}


/**
 * @brief create an draw single card
 * Column and row is specified by card.position
 * @param card: game card
 * @returns card label reference
 */
PlayLabel *Pasians::drawCard(Card *card, QSize &cardSize)
{
    PlayLabel *lbl = new PlayLabel(card, cardSize);

    PlayWrapper *item = new PlayWrapper(lbl);

    scene->addItem(item);

    return lbl;
}

/**
 * @brief generate new game
 */
GGame *Pasians::generateGame()
{
    GGame *game = new GGame(this);
    game->setup();
    return game;
}

/**
 * @brief Pasians::showGame print out single game within internal layout
 * @param game particular game
 * @param layout inner layout
**/
void Pasians::showGame(GGame *game, Layout &layout)
{
    PlayLabel *w;
    QSize &cardSize = layout.cardSize;

    if (game->initialized()) { // allready initialized

        int z = 0;
        for (auto card: game->pickPile.cards) {
            z++;
            w = static_cast<PlayLabel *>(card->parent);
            w->setSize(cardSize);
            w->setPos(layout.pick);
            w->setZ(z);
        }

        w = static_cast<PlayLabel *>(game->pickPile.placeHolder());
        w->setPos(layout.pick);
        w->setSize(cardSize);

        z = 0;
        for (auto card: game->dropPile.cards) {
            z++;
            w = static_cast<PlayLabel *>(card->parent);
            w->setSize(cardSize);
            w->setPos(layout.drop);
            w->setZ(z);
        }

        w = static_cast<PlayLabel *>(game->dropPile.placeHolder());
        w->setPos(layout.drop);
        w->setSize(cardSize);

        QPoint botPos = layout.bot;
        for (auto &pile: game->bottomPiles) {
            z = 0;

            w = static_cast<PlayLabel *>(pile.placeHolder());
            w->setPos(botPos);
            w->setSize(cardSize);

            for (auto card: pile.cards) {
                z++;
                w = static_cast<PlayLabel *>(card->parent);
                w->setSize(cardSize);
                w->setPos(botPos);
                w->setZ(z);
                botPos.setY(botPos.y() + layout.cardHeight / cardOffsetMod);
            }

            botPos.setX(botPos.x() + layout.cardWidth + layout.wspace);
            botPos.setY(layout.bot.y());
        }

        QPoint topPos = layout.top;
        for (auto &pile: game->topPiles) {
            z = 0;

            w = static_cast<PlayLabel *>(pile.placeHolder());
            w->setPos(topPos);
            w->setSize(cardSize);

            for (auto card: pile.cards) {
                z++;
                w = static_cast<PlayLabel *>(card->parent);
                w->setSize(cardSize);
                w->setPos(topPos);
                w->setZ(z);
            }

            topPos.setX(topPos.x() + layout.cardWidth + layout.wspace);
        }
    } else { // game initialization

        for (auto card: game->pickPile.cards) {
            w = drawCard(card, cardSize);
            w->setPos(layout.pick);
            w->setContext(&game->pickPile, game);
        }

        for (auto card: game->dropPile.cards) {
            w = drawCard(card, cardSize);
            w->setPos(layout.drop);
            w->setContext(&game->dropPile, game);
        }

        QPoint botPos = layout.bot;
        for (auto &pile: game->bottomPiles) {
            for (auto card: pile.cards) {
                w = drawCard(card, cardSize);
                w->setPos(botPos);
                w->setContext(&pile, game);
            }
            botPos.setX(botPos.x() + layout.cardWidth + layout.wspace);
        }

        QPoint topPos = layout.top;
        for (auto &pile: game->topPiles) {
            for (auto card: pile.cards) {
                w = drawCard(card, cardSize);
                w->setPos(botPos);
                w->setContext(&pile, game);
            }
            topPos.setX(topPos.x() + layout.cardWidth + layout.wspace);
        }

        game->setupPlaceHolders(layout, scene);

        game->start();

        showGame(game, layout);
    }
}

/**
* @brief cleanup game
**/
void Pasians::finalizeGame(GGame *game)
{
    if (!game->initialized()) {
        redraw();
        return;
    }

    PlayLabel *l = static_cast<PlayLabel *>(game->pickPile.placeHolder());
    scene->removeItem(l->wrapper());
    for (auto card: game->pickPile.cards) {
        l = static_cast<PlayLabel *>(card->parent);
        scene->removeItem(l->wrapper());
    }

    l = static_cast<PlayLabel *>(game->dropPile.placeHolder());
    scene->removeItem(l->wrapper());
    for (auto card: game->dropPile.cards) {
        l = static_cast<PlayLabel *>(card->parent);
        scene->removeItem(l->wrapper());
    }

    for (auto &pile: game->topPiles) {
        l = static_cast<PlayLabel *>(pile.placeHolder());
        scene->removeItem(l->wrapper());
        for (auto card: pile.cards) {
            l = static_cast<PlayLabel *>(card->parent);
            scene->removeItem(l->wrapper());
        }
    }

    for (auto &pile: game->bottomPiles) {
        l = static_cast<PlayLabel *>(pile.placeHolder());
        scene->removeItem(l->wrapper());
        for (auto card: pile.cards) {
            l = static_cast<PlayLabel *>(card->parent);
            scene->removeItem(l->wrapper());
        }
    }

    redraw();
}

void Pasians::on_actionNew_triggered()
{
    GGame *game = generateGame();
    games.push_back(game);

    redraw();
}

void Pasians::on_actionQuit_4_triggered()
{
    const auto &it = games.begin() + 3;
    GGame *game = *it;
    games.erase(it);
    finalizeGame(game);
}

void Pasians::on_actionQuit_3_triggered()
{
    const auto &it = games.begin() + 2;
    GGame *game = *it;
    games.erase(it);
    finalizeGame(game);;
}

void Pasians::on_actionQuit_2_triggered()
{
    const auto &it = games.begin() + 1;
    GGame *game = *it;
    games.erase(it);
    finalizeGame(game);
}

void Pasians::on_actionQuit_triggered()
{
    const auto &it = games.begin();
    GGame *game = *it;
    games.erase(it);
    finalizeGame(game);
}

void Pasians::on_actionUndo_triggered()
{
    GGame *game = games[0];
    game->performUndo();
    redraw();
}

void Pasians::on_actionUndo_2_triggered()
{
    GGame *game = games[1];
    game->performUndo();
    redraw();
}

void Pasians::on_actionUndo_3_triggered()
{
    GGame *game = games[2];
    game->performUndo();
    redraw();
}


void Pasians::on_actionUndo_4_triggered()
{
    GGame *game = games[3];
    game->performUndo();
    redraw();
}

void Pasians::on_actionHint_triggered()
{
    GGame *game = games[0];
    game->performHint();
    redraw();
}

void Pasians::on_actionHint_2_triggered()
{
    GGame *game = games[1];
    game->performHint();
    redraw();
}

void Pasians::on_actionHint_3_triggered()
{
    GGame *game = games[2];
    game->performHint();
    redraw();
}

void Pasians::on_actionHint_4_triggered()
{
    GGame *game = games[3];
    game->performHint();
    redraw();
}

QString Pasians::loadFile()
{
    return QFileDialog::getOpenFileName(
                this,
                "Load game",
                "",
                "Pasians game (*.pas)");
}

QString Pasians::saveFile()
{
    return QFileDialog::getSaveFileName(
                this,
                "Save game",
                "game.pas",
                "Pasians game (*.pas)");
}

void Pasians::on_actionSave_triggered()
{
    QString f = saveFile();
    if (f.isEmpty()) {
        return;
    }
    GGame *game = games[0];
    game->save(f.toStdString());
}

void Pasians::on_actionLoad_triggered()
{
    QString f = loadFile();
    if (f.isEmpty()) {
        return;
    }
    GGame *loaded = new GGame(this);
    loaded->setup();
    loaded->load(f.toStdString());
    finalizeGame(games[0]);
    games[0] = loaded;
    redraw();
}

void Pasians::on_actionSave_2_triggered()
{
    QString f = saveFile();
    if (f.isEmpty()) {
        return;
    }
    GGame *game = games[1];
    game->save(f.toStdString());
}

void Pasians::on_actionLoad_2_triggered()
{
    QString f = loadFile();
    if (f.isEmpty()) {
        return;
    }
    GGame *loaded = new GGame(this);
    loaded->setup();
    loaded->load(f.toStdString());
    finalizeGame(games[1]);
    games[1] = loaded;
    redraw();
}

void Pasians::on_actionSave_3_triggered()
{
    QString f = saveFile();
    if (f.isEmpty()) {
        return;
    }
    GGame *game = games[2];
    game->save(f.toStdString());
}

void Pasians::on_actionLoad_3_triggered()
{
    QString f = loadFile();
    if (f.isEmpty()) {
        return;
    }
    GGame *loaded = new GGame(this);
    loaded->setup();
    loaded->load(f.toStdString());
    finalizeGame(games[2]);
    games[2] = loaded;
    redraw();
}

void Pasians::on_actionSave_4_triggered()
{
    QString f = saveFile();
    if (f.isEmpty()) {
        return;
    }
    GGame *game = games[3];
    game->save(f.toStdString());
}

void Pasians::on_actionLoad_4_triggered()
{
    QString f = loadFile();
    if (f.isEmpty()) {
        return;
    }
    GGame *loaded = new GGame(this);
    loaded->setup();
    loaded->load(f.toStdString());
    finalizeGame(games[3]);
    games[3] = loaded;
    redraw();
}
