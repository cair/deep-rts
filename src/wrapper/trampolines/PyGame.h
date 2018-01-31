//
// Created by Per-Arne on 23.02.2017.
//

#pragma once

#include "../../Game.h"



class PyGame: public Game {
    using Game::Game;


    virtual void _render() override {
        PYBIND11_OVERLOAD(void, Game, _render, );
    }

    virtual void _caption() override {
        PYBIND11_OVERLOAD(void, Game, _caption, );
    }

    virtual void _update() override {
        PYBIND11_OVERLOAD(void, Game, _update, );
    }

    virtual void _reset() override {
        PYBIND11_OVERLOAD(void, Game, _reset, );
    }

};
