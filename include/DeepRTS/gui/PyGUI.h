//
// Created by per on 06.06.18.
//
#pragma once

#include <pybind11/pytypes.h>
#include "BaseGUI.h"

class Tile;

class PyGUI: public BaseGUI{
private:
    pybind11::object gui;
    pybind11::object gui_attr_on_tile_change;

    pybind11::object gui_attr_render;
    pybind11::object gui_attr_view;

    static void initDependencies();
    static void initArgv();
    static void initCython();
    void initGUI();

public:
    cv::Mat renderData; // TODO
    PyGUI(Game&);

    ~PyGUI();

    const cv::Mat& render()const override;
    void onTileChange(const Tile &tile);
};