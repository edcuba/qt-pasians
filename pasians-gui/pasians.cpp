#include "pasians.h"
#include "ui_pasians.h"
#include <QSize>
#include <QResizeEvent>
#include <QGraphicsScene>
#include <vector>
#include <iostream>
#include <QMessageBox>
#include "playwrapper.h"

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

void Pasians::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    showGames();
}

Pasians::~Pasians()
{
    delete ui;
    for (auto &game: games) {
        delete game;
    }
}

static void showFinish()
{
    QMessageBox msgBox;
    msgBox.setText("You won!");
    msgBox.exec();
}

void Pasians::redraw()
{
    for (vector<GGame *>::iterator it = games.begin(); it != games.end(); ++it) {
        GGame *game = *it;
        if (game->done()) {
            showFinish();
            games.erase(it);
        }
    }
    showGames();
}

/**
 * @brief Pasians::showGames show all games in global layout
 * @param layout global layout
 */
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
 * @brief Pasians::drawCard draw single card
 *
 * Column and row is specified by card.position
 * @param card game card
 * @returns card label reference
 */
PlayLabel *Pasians::drawCard(Card &card, QSize &cardSize)
{
    PlayLabel *lbl = new PlayLabel(&card, cardSize);

    PlayWrapper *item = new PlayWrapper(lbl);

    scene->addItem(item);

    return lbl;
}

/**
 * @brief Pasians::generateGame generate new game
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
 */
void Pasians::showGame(GGame *game, Layout &layout)
{
    PlayLabel *w;
    QSize &cardSize = layout.cardSize;

    if (game->initialized()) { // allready initialized

        int z = 0;
        for (auto &card: game->pickPile.cards) {
            z++;
            w = static_cast<PlayLabel *>(card.parent);
            w->setSize(cardSize);
            w->setPos(layout.pick);
            w->setZ(z);
        }

        w = static_cast<PlayLabel *>(game->pickPile.placeHolder());
        w->setPos(layout.pick);
        w->setSize(cardSize);

        z = 0;
        for (auto &card: game->dropPile.cards) {
            z++;
            w = static_cast<PlayLabel *>(card.parent);
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

            for (auto &card: pile.cards) {
                z++;
                w = static_cast<PlayLabel *>(card.parent);
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

            for (auto &card: pile.cards) {
                z++;
                w = static_cast<PlayLabel *>(card.parent);
                w->setSize(cardSize);
                w->setPos(topPos);
                w->setZ(z);
            }

            topPos.setX(topPos.x() + layout.cardWidth + layout.wspace);
        }
    } else { // game initialization

        for (auto &card: game->pickPile.cards) {
            w = drawCard(card, cardSize);
            w->setPos(layout.pick);
            w->setContext(&game->pickPile, game);
        }

        for (auto &card: game->dropPile.cards) {
            w = drawCard(card, cardSize);
            w->setPos(layout.drop);
            w->setContext(&game->dropPile, game);
        }

        QPoint botPos = layout.bot;
        for (auto &pile: game->bottomPiles) {
            for (auto &card: pile.cards) {
                w = drawCard(card, cardSize);
                w->setPos(botPos);
                w->setContext(&pile, game);
            }
            botPos.setX(botPos.x() + layout.cardWidth + layout.wspace);
        }

        QPoint topPos = layout.top;
        for (auto &pile: game->topPiles) {
            for (auto &card: pile.cards) {
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

void Pasians::finalizeGame(GGame *game)
{
    if (!game->initialized()) {
        redraw();
        return;
    }

    PlayLabel *l = static_cast<PlayLabel *>(game->pickPile.placeHolder());
    scene->removeItem(l->wrapper());
    for (auto &card: game->pickPile.cards) {
        l = static_cast<PlayLabel *>(card.parent);
        scene->removeItem(l->wrapper());
    }

    l = static_cast<PlayLabel *>(game->dropPile.placeHolder());
    scene->removeItem(l->wrapper());
    for (auto &card: game->dropPile.cards) {
        l = static_cast<PlayLabel *>(card.parent);
        scene->removeItem(l->wrapper());
    }

    for (auto &pile: game->topPiles) {
        l = static_cast<PlayLabel *>(pile.placeHolder());
        scene->removeItem(l->wrapper());
        for (auto &card: pile.cards) {
            l = static_cast<PlayLabel *>(card.parent);
            scene->removeItem(l->wrapper());
        }
    }

    for (auto &pile: game->bottomPiles) {
        l = static_cast<PlayLabel *>(pile.placeHolder());
        scene->removeItem(l->wrapper());
        for (auto &card: pile.cards) {
            l = static_cast<PlayLabel *>(card.parent);
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
    games[0]->undo();
    showGames();
}

void Pasians::on_actionUndo_2_triggered()
{
    games[1]->undo();
    showGames();
}

void Pasians::on_actionUndo_3_triggered()
{
    games[2]->undo();
    showGames();
}


void Pasians::on_actionUndo_4_triggered()
{
    games[3]->undo();
    showGames();
}
