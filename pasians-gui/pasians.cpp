#include "pasians.h"
#include "ui_pasians.h"
#include <QPixmap>
#include <QSize>
#include <QDebug>
#include <QResizeEvent>
#include <QGridLayout>
#include <QSplitter>
#include "../src/paslib.h"

#include <vector>
#include <iostream>



Pasians::Pasians(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Pasians)
{
    ui->setupUi(this);

    GGame *game = generateGame();
    games.push_back(game);

    QSize s = size();

    Layout layout(s.height(), s.width());

    showGame(game, layout);
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

QSize GameCard::minimumSizeHint() const {
    return QSize(size().width(), size().height() / 5);
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
GameCard *Pasians::drawCard(Card &card, QSize &cardSize)
{
    GameCard *lbl = new GameCard();

    lbl->setFixedSize(cardSize);

    ui->gameLayout->addWidget(lbl, card.x, card.y, Qt::AlignTop);

    string type = hashCard(card);

    string path = cardImg.find(type)->second;

    QPixmap img(path.c_str());

    lbl->setPixmap(img);

    lbl->setScaledContents(true);
    return lbl;
}

/**
 * @brief Pasians::drawChildCard draw single card at specified position
 * @param card game card object
 * @param cardSize size of the card
 * @param perent parent layout
 * @return card label reference
 */
GameCard *Pasians::drawChildCard(Card &card, QSize &cardSize, QSplitter *parent)
{
    GameCard *lbl = new GameCard();

    string type = hashCard(card);

    string path = cardImg.find(type)->second;

    QPixmap img(path.c_str());

    lbl->setPixmap(img);

    lbl->setScaledContents(true);

    lbl->setFixedSize(cardSize);

    parent->addWidget(lbl);

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

    for (int i = 0; i < 7; ++i) {
        ui->gameLayout->setColumnMinimumWidth(i, layout.cardWidth);
    }

    ui->gameLayout->setRowMinimumHeight(0, layout.cardHeight);

    ui->gameLayout->setRowMinimumHeight(1, 3.4 * layout.cardHeight);

    ui->gameLayout->setHorizontalSpacing(layout.wspace);
    ui->gameLayout->setVerticalSpacing(layout.cardHeight / 5);

    if (game->initialized) {
        for (auto &card: game->gameCards) {
            card.second->setFixedSize(cardSize);
        }
        int pilenum = 0;
        for (auto &pile: game->bottomPiles) {
            for (auto &box: bottomHolders) {
                int size = pile.cards.size();
                int maxh = (size + 5) * (cardSize.height() / 5);

                box->setMaximumHeight(maxh);
                box->setFixedWidth(cardSize.width());
            }
            pilenum++;
        }
    } else {

        for (auto &card: game->pickPile.cards) {
            string type = hashCard(card);
            card.x = 0;
            card.y = 0;
            game->gameCards[type] = drawCard(card, cardSize);
        }

        for (auto &card: game->dropPile.cards) {
            string type = hashCard(card);
            card.x = 0;
            card.y = 1;
            game->gameCards[type] = drawCard(card, cardSize);
        }

        //TODO generate top pile cardholders (borders)

        int pilenum = 0;
        for (auto &pile: game->bottomPiles) {
            QSplitter *box = new QSplitter(Qt::Vertical);
            bottomHolders.push_back(box);

            int size = pile.cards.size();
            int maxh = (size + 5) * (cardSize.height() / 5);

            box->setMaximumHeight(maxh);
            box->setFixedWidth(cardSize.width());

            ui->gameLayout->addWidget(box, 1, pilenum);

            for (auto &card: pile.cards) {
                string type = hashCard(card);
                game->gameCards[type] = drawChildCard(card, cardSize, box);
            }
            pilenum++;
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

    wspace = w * 2.5;

    cardWidth = 12 * w;

    cardHeight = cardWidth * 1.452; // 500 x 726
}
