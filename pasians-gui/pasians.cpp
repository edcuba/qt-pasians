#include "pasians.h"
#include "ui_pasians.h"
#include <QSize>
#include <QResizeEvent>
#include <QGraphicsScene>
#include <vector>
#include <iostream>

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

    QSize s = size();

    Layout layout(s);

    showGame(game, layout);

    view->show();

    view->setMouseTracking(true);
    this->setMouseTracking(true);
    ui->gameBoard->setMouseTracking(true);
}

void Pasians::resizeEvent(QResizeEvent* event)
{
    Layout layout(event->size());

    view->setSceneRect(scene->sceneRect());

    view->centerOn(0, 0);

    showGames(layout);
}

Pasians::~Pasians()
{
    delete ui;
    for (auto &game: games) {
        delete game;
    }
}

void Pasians::redraw()
{
    showGames(activeLayout);
}

/**
 * @brief Pasians::showGames show all games in global layout
 * @param layout global layout
 */
void Pasians::showGames(Layout &layout)
{
    if (games.size() == 1) {
        showGame(games.front(), layout);
    }
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
    PlayLabel *lbl = new PlayLabel(card, cardSize);

    lbl->setAttribute(Qt::WA_TranslucentBackground);


    QGraphicsProxyWidget *w = scene->addWidget(lbl);

    lbl->setWrapper(w);

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
    QSize cardSize(layout.cardWidth, layout.cardHeight);

    PlayLabel *w;
    if (game->initialized()) {

        int z = 0;
        for (auto &card: game->pickPile.cards) {
            z++;
            w = (PlayLabel *) card.parent;
            w->setPos(layout.pick);
            w->setZ(z);
        }

        z = 0;
        for (auto &card: game->dropPile.cards) {
            z++;
            w = (PlayLabel *) card.parent;
            w->setPos(layout.drop);
            w->setZ(z);
        }

        QPoint botPos = layout.bot;
        for (auto &pile: game->bottomPiles) {
            z = 0;
            for (auto &card: pile.cards) {
                z++;
                w = (PlayLabel *) card.parent;
                w->setPos(botPos);
                botPos.setY(botPos.y() + layout.cardHeight / cardOffsetMod);
                w->setZ(z);
            }
            botPos.setX(botPos.x() + layout.cardWidth + layout.wspace);
            botPos.setY(layout.bot.y());
        }

        QPoint topPos = layout.top;
        for (auto &pile: game->topPiles) {
            z = 0;
            for (auto &card: pile.cards) {
                z++;
                w = (PlayLabel *) card.parent;
                w->setPos(topPos);
                w->setZ(z);
            }
            topPos.setX(topPos.x() + layout.cardWidth + layout.wspace);
        }

    } else {

        for (auto &card: game->pickPile.cards) {
            w = drawCard(card, cardSize);
            w->setPos(layout.pick);
            w->setContext(game->pickPile, game);
        }

        for (auto &card: game->dropPile.cards) {
            w = drawCard(card, cardSize);
            w->setPos(layout.drop);
            w->setContext(game->dropPile, game);
        }

        QPoint botPos = layout.bot;
        for (auto &pile: game->bottomPiles) {
            for (auto &card: pile.cards) {
                w = drawCard(card, cardSize);
                w->setPos(botPos);
                w->setContext(pile, game);
            }
            botPos.setX(botPos.x() + layout.cardWidth + layout.wspace);
        }

        QPoint topPos = layout.top;
        for (auto &pile: game->topPiles) {
            for (auto &card: pile.cards) {
                w = drawCard(card, cardSize);
                w->setPos(botPos);
                w->setContext(pile, game);
            }
            topPos.setX(topPos.x() + layout.cardWidth + layout.wspace);
        }

        view->setSceneRect(scene->sceneRect());
        game->start();
    }
    activeLayout = layout;
}
