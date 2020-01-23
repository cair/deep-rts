//
// Created by per on 06.06.18.
//
#pragma once

#include <pybind11/pytypes.h>
#include "../Game.h"

class PyGUI {
private:
    pybind11::object gui;
    pybind11::object gui_attr_on_tile_change;

    pybind11::object gui_attr_render;
    pybind11::object gui_attr_view;

    static void initDependencies();
    static void initArgv();
    static void initCython();
    void initGUI();
    Game &game;
public:
    explicit PyGUI(Game &game);
    ~PyGUI();

    void view();
    void onTileChange(Tile &tile);
};