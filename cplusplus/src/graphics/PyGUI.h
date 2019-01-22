//
// Created by per on 06.06.18.
//

#ifndef DEEPRTS_PYGUI_H
#define DEEPRTS_PYGUI_H


#include <pybind11/pytypes.h>
#include "../Game.h"

class PyGUI {
private:
    pybind11::object gui;
    pybind11::object gui_attr_render;
    pybind11::object gui_attr_view;
    void init_dependencies();
    void init_argv();
    void init_cython();
    void init_gui();
    Game &game;
public:
    PyGUI(Game &game);

    void render();
    void view();
    void capture();

    ~PyGUI();


};


#endif //DEEPRTS_PYGUI_H
