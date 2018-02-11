//
// Created by per on 11.02.18.
//

#ifndef DEEPRTS_GUIGAME_H
#define DEEPRTS_GUIGAME_H

#include "../Game.h"
#include "GUI.h"


class GUIGame: public Game{
    GUI gui;

public:


    GUIGame();

    virtual void _render();

};


#endif //DEEPRTS_GUIGAME_H
