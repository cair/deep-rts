//
// Created by per on 11.02.18.
//

#ifndef DEEPRTS_GUIGAME_H
#define DEEPRTS_GUIGAME_H

#include "../Game.h"
#include "GUI.h"
#include <iostream>

class GUIGame: public Game{
    GUI gui;

public:


    GUIGame(std::string map_file);

    virtual void _render();

};


#endif //DEEPRTS_GUIGAME_H
