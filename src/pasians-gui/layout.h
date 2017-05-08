/**
* layout.h
* Pasians GUI - ICP 2016/2017
* Authors:
*   Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
*   Ondrej Kurak <xkurak00@stud.fit.vutbr.cz>
**/

#ifndef LAYOUT_H
#define LAYOUT_H

#include <QSize>
#include <QPoint>

class Layout
{
public:
    Layout();
    Layout(const QSize &size);
    Layout(const QSize &size, const QPoint &offset);
    int cardWidth;
    int cardHeight;
    int wspace;

    QSize cardSize;

    QPoint pick;
    QPoint drop;

    QPoint bot;
    QPoint top;

    QSize lsize;
protected:
    void setupLayout(const QSize &size, const QPoint &offset);
};

#endif // LAYOUT_H
