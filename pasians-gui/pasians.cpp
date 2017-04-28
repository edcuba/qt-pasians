#include "pasians.h"
#include "ui_pasians.h"
#include <QPixmap>
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
 * @brief Pasians::hashCard Calculate card identificator
 * @param card game card
 * @return card identificator
 */
string Pasians::hashCard(Card &card)
{
    return to_string((int) card.type) + "-" + to_string((int) card.color);
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
    PlayLabel *lbl = new PlayLabel();

    string type = "none";

    if (card.visible){
        type = hashCard(card);
    }

    string path;

    path = cardImg.find(type)->second;

    QPixmap img(path.c_str());

    lbl->setPixmap(img);

    lbl->setScaledContents(true);

    lbl->setFixedSize(cardSize);

    QGraphicsProxyWidget *w = scene->addWidget(lbl);

    lbl->setContext(w, this);

    return lbl;
}

/**
 * @brief Pasians::generateGame generate new game
 */
GGame *Pasians::generateGame()
{
    GGame *game = new GGame();
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

    view->centerOn(0, 0);

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
            w->setCard(card, game->pickPile);
        }

        for (auto &card: game->dropPile.cards) {
            w = drawCard(card, cardSize);
            w->setPos(layout.drop);
            w->setCard(card, game->dropPile);
        }

        QPoint botPos = layout.bot;
        for (auto &pile: game->bottomPiles) {
            for (auto &card: pile.cards) {
                w = drawCard(card, cardSize);
                w->setPos(botPos);
                w->setCard(card, pile);
            }
            botPos.setX(botPos.x() + layout.cardWidth + layout.wspace);
        }

        QPoint topPos = layout.top;
        for (auto &pile: game->topPiles) {
            for (auto &card: pile.cards) {
                w = drawCard(card, cardSize);
                w->setPos(botPos);
                w->setCard(card, pile);
            }
            topPos.setX(topPos.x() + layout.cardWidth + layout.wspace);
        }

        game->start();
    }
    activeLayout = layout;
}
