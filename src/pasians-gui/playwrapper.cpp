/**
* playwrapper.cpp
* Pasians GUI - ICP 2016/2017
* Authors:
*   Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
*   Ondrej Kurak <xkurak00@stud.fit.vutbr.cz>
**/

#include "playwrapper.h"


/**
* @brief Create new wrapper for game card
**/
PlayWrapper::PlayWrapper(PlayLabel *l)
{
    setWidget(l);
    l->setWrapper(this);
}


/**
* @brief Adjuct size of card wrapper
**/
void PlayWrapper::adjust()
{
    /* **sigh**... just a dirty workaround
     * I couldn't make it sync with its widget
    */
    QWidget *w = widget();
    setWidget(0);
    setWidget(w);
}
