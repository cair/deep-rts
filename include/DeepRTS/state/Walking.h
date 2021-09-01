//
// Created by Per-Arne on 24.02.2017.
//
#pragma once

#include "BaseState.h"
#include "../util/JPS.h"


class Tilemap;
class Game;
class Walking: public BaseState {
    JPS::Searcher<Tilemap&> search;

public:
    explicit Walking(Game & game);
    void update(Unit & unit) override;
    void init(Unit & unit) override;
    void end(Unit & unit) override;
};

