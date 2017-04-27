#include "pasians.h"
#include "ui_pasians.h"
#include <QPixmap>
#include <QSize>
#include <QResizeEvent>
#include "../src/paslib.h"
#include <QGraphicsScene>
#include <vector>
#include <iostream>
#include <QSizePolicy>
#include <QGraphicsSceneMouseEvent>



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

void PlayLabel::mousePressEvent(QMouseEvent * event)
{
    Q_UNUSED(event);
}

void PlayLabel::mouseReleaseEvent(QMouseEvent * event)
{
    Q_UNUSED(event);
    pasians->redraw();
}

void PlayLabel::mouseMoveEvent(QMouseEvent *event)
{
    parent->setPos(x() + event->x() - size().width() / 2, y() + event->y() - size().height() / 2);
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

GGame::~GGame()
{
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
QGraphicsProxyWidget *Pasians::drawCard(Card &card, QSize &cardSize)
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

    lbl->parent = w;
    lbl->pasians = this;

    return w;
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

    QGraphicsProxyWidget *w;
    if (game->initialized) {

        for (auto &card: game->pickPile.cards) {
            w = (QGraphicsProxyWidget *) card.parent;
            w->setPos(layout.pick);
        }

        for (auto &card: game->dropPile.cards) {
            w = (QGraphicsProxyWidget *) card.parent;
            w->setPos(layout.drop);
        }

        QPoint botPos = layout.bot;
        for (auto &pile: game->bottomPiles) {
            for (auto &card: pile.cards) {
                w = (QGraphicsProxyWidget *) card.parent;
                w->setPos(botPos);
                botPos.setY(botPos.y() + layout.cardHeight / 5);
            }
            botPos.setX(botPos.x() + layout.cardWidth + layout.wspace);
            botPos.setY(layout.bot.y());
        }

        QPoint topPos = layout.top;
        for (auto &pile: game->topPiles) {
            for (auto &card: pile.cards) {
                w = (QGraphicsProxyWidget *) card.parent;
                w->setPos(topPos);
            }
            topPos.setX(topPos.x() + layout.cardWidth + layout.wspace);
        }

    } else {

        for (auto &card: game->pickPile.cards) {
            w = drawCard(card, cardSize);
            w->setPos(layout.pick);
            card.parent = w;
        }

        for (auto &card: game->dropPile.cards) {
            w = drawCard(card, cardSize);
            w->setPos(layout.drop);
            card.parent = w;
        }

        QPoint botPos = layout.bot;
        for (auto &pile: game->bottomPiles) {
            for (auto &card: pile.cards) {
                w = drawCard(card, cardSize);
                w->setPos(botPos);
                card.parent = w;
            }
            botPos.setX(botPos.x() + layout.cardWidth + layout.wspace);
        }

        QPoint topPos = layout.top;
        for (auto &pile: game->topPiles) {
            for (auto &card: pile.cards) {
                w = drawCard(card, cardSize);
                w->setPos(botPos);
                card.parent = w;
            }
            topPos.setX(topPos.x() + layout.cardWidth + layout.wspace);
        }

        game->initialized = true;
    }
    activeLayout = layout;
}


Layout::Layout() {}

Layout::Layout(const QSize &size)
{
    int height = size.height();
    int width = size.width();

    int w = width / 100;

    if (w * 14.52 * 5 > height) {
        w = 1.5 * height / 100;
    }
    cardWidth = 12 * w;

    wspace = 2 * w;

    cardHeight = cardWidth * 1.452; // 500 x 726

    pick.setX(- width / 2 + cardWidth / 2);
    pick.setY(- height / 2);

    drop.setX(pick.x() + cardWidth + wspace);
    drop.setY(pick.y());

    bot.setX(pick.x());
    bot.setY(-cardHeight);

    top.setX(drop.x() + 2 * (cardWidth + wspace));
    top.setY(pick.y());
}
