#include "playwrapper.h"

PlayWrapper::PlayWrapper(PlayLabel *l)
{
    setWidget(l);
    l->setWrapper(this);
}


void PlayWrapper::adjust()
{
    /* **sigh**... just a dirty workaround
     * I couldn't make it sync with its widget
    */
    QWidget *w = widget();
    setWidget(0);
    setWidget(w);
}
