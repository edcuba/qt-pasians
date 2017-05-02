#ifndef LAYOUT_H
#define LAYOUT_H

#include <QSize>
#include <QPoint>

class Layout
{
public:
    Layout();
    Layout(const QSize &size);
    int cardWidth;
    int cardHeight;
    int wspace;

    QSize cardSize;

    QPoint pick;
    QPoint drop;

    QPoint bot;
    QPoint top;
};

#endif // LAYOUT_H
