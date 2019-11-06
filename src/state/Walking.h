//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_WALKING_H
#define WARC2SIM_WALKING_H


#include "BaseState.h"
#include "../util/JPS.h"


class Tilemap;
class Game;
class Walking: public BaseState {
    JPS::Searcher<Tilemap&> search;

public:
    Walking(Game & game);
    virtual void update(Unit & unit);
    virtual void init(Unit & unit);
    virtual void end(Unit & unit);
};


#endif //WARC2SIM_WALKING_H
