//
// Created by per on 23.03.18.
//

#ifndef DEEPRTS_GAMESTATE_H
#define DEEPRTS_GAMESTATE_H
#include "StateChange.h"
#include <list>


class GameState {
    std::list<StateChange> changes;
};


#endif //DEEPRTS_GAMESTATE_H
