/**
* playwrapper.h
* Pasians GUI - ICP 2016/2017
* Authors:
*   Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
*   Ondrej Kurak <xkurak00@stud.fit.vutbr.cz>
**/

#ifndef PLAYWRAPPER_H
#define PLAYWRAPPER_H

#include <QGraphicsProxyWidget>
#include <QSize>

class PlayWrapper;

#include "playlabel.h"

class PlayWrapper : public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    PlayWrapper(PlayLabel *l);
    void adjust();
};

#endif // PLAYWRAPPER_H
