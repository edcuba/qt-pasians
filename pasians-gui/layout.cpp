#include "layout.h"
#include <QSize>
#include <QPoint>

Layout::Layout() {}

Layout::Layout(const QSize &size, const QPoint &offset)
{
    setupLayout(size, offset);
}

Layout::Layout(const QSize &size)
{
    setupLayout(size, QPoint());
}

void Layout::setupLayout(const QSize &size, const QPoint &offset)
{

    int ox = offset.x();
    int oy = offset.y();

    int height = size.height();
    int width = size.width();

    int w = width / 100;

    if (w * 14.52 * 5 > height) {
        w = 1.42 * height / 100;
    }

    lsize = size;

    cardWidth = 11 * w;

    wspace = 2 * w;

    cardHeight = cardWidth * 1.452; // 500 x 726

    pick.setX(- width / 2 + cardWidth / 2 + ox);
    pick.setY(- height / 2 + oy);

    drop.setX(pick.x() + cardWidth + wspace);
    drop.setY(pick.y());

    bot.setX(pick.x());
    bot.setY(pick.y() + 1.1 * cardHeight);

    top.setX(drop.x() + 2 * (cardWidth + wspace));
    top.setY(pick.y());

    cardSize.setWidth(cardWidth);
    cardSize.setHeight(cardHeight);
}


