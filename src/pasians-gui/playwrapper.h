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
