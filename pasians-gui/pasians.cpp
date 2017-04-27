#include "pasians.h"
#include "ui_pasians.h"
#include <QPixmap>
#include <QSize>
#include <QResizeEvent>
#include "../src/paslib.h"
#include <QGraphicsScene>
#include <QLabel>

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

    Layout layout(s.height(), s.width());

    showGame(game, layout);

    view->show();
}

void Pasians::resizeEvent(QResizeEvent* event)
{
    Layout layout(event->size().height(), event->size().width());

    showGames(layout);
}

Pasians::~Pasians()
{
    delete ui;
    for (auto &game: games) {
        delete game;
    }
}

GGame::~GGame()
{
    for (auto &card: gameCards) {
        delete card.second;
    }
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
QLabel *Pasians::drawCard(Card &card, QSize &cardSize)
{
    QLabel *lbl = new QLabel();

    string type = hashCard(card);

    string path = cardImg.find(type)->second;

    QPixmap img(path.c_str());

    lbl->setPixmap(img);

    lbl->setScaledContents(true);

    lbl->setFixedSize(cardSize);

    return lbl;
}

/**
 * @brief Pasians::showGame print out single game within internal layout
 * @param game particular game
 * @param layout inner layout
 */
void Pasians::showGame(GGame *game, Layout &layout)
{
    QSize cardSize(layout.cardWidth, layout.cardHeight);
    QGraphicsProxyWidget *w;
    if (game->initialized) {
        for (auto &card: game->gameCards) {
            w = card.second;
            w->widget()->setFixedSize(cardSize);
        }

    } else {

        QLabel *gc;

        for (auto &card: game->pickPile.cards) {
            string type = hashCard(card);
            gc = drawCard(card, cardSize);
            w = scene->addWidget(gc);
            w->setPos(layout.pick);
            game->gameCards[type] = w;
        }

        for (auto &card: game->dropPile.cards) {
            string type = hashCard(card);
            gc = drawCard(card, cardSize);
            w = scene->addWidget(gc);
            w->setPos(layout.drop);
            game->gameCards[type] = w;
        }

        QPoint botPos = layout.bot;
        for (auto &pile: game->bottomPiles) {
            for (auto &card: pile.cards) {
                string type = hashCard(card);
                gc = drawCard(card, cardSize);
                w = scene->addWidget(gc);
                w->setPos(botPos);
                game->gameCards[type] = w;
            }
            botPos.setX(botPos.x() + layout.cardWidth + layout.wspace);
        }

        game->initialized = true;
    }
}


Layout::Layout(int height, int width)
{
    int w = width / 100;

    if (w * 14.52 * 5 > height) {
        w = 1.5 * height / 100;
    }
    cardWidth = 12 * w;

    wspace = 2 * w;

    cardHeight = cardWidth * 1.452; // 500 x 726

    pick.setX(- width / 2);
    pick.setY(- height / 2);

    drop.setX(pick.x() + cardWidth + wspace);
    drop.setY(pick.y());

    bot.setX(- width / 2);
    bot.setY(height / 5 - cardHeight);

    top.setX(drop.x() + 2 * (cardWidth + wspace));
    top.setY(pick.y());
}
