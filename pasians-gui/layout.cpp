#include "layout.h"
#include <QSize>
#include <QPoint>

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

    cardSize.setWidth(cardWidth);
    cardSize.setHeight(cardHeight);

}
