//
// Created by per on 28.01.18.
//
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#ifndef EMBEDDED
#include "utilities/ndarray_converter.h"
#endif

namespace py = pybind11;

void init_Constants(py::module &);
void init_BaseState(py::module &);
void init_UnitManager(py::module &);
void init_Unit(py::module &);
void init_Tilemap(py::module &);
void init_Tile(py::module &);
void init_Player(py::module &);
void init_Game(py::module &);
void init_Map(py::module &);
void init_Config(py::module &);
void init_Random(py::module &);
void init_Webserver(py::module &);
void init_scenarios(py::module &);
void init_version(py::module &m) {
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}


#ifndef EMBEDDED



PYBIND11_MODULE(Engine, m) {
    NDArrayConverter::init_numpy();
    m.doc() = "DeepRTS Engine";
    init_Random(m);
    init_Constants(m);
    init_BaseState(m);
    init_Unit(m);
    init_Map(m);
    init_Tile(m);
    init_Tilemap(m);
    init_Game(m);
    init_Player(m);
    init_version(m);
    init_Config(m);
    init_UnitManager(m);
    init_Webserver(m);
    init_scenarios(m);
}
#else
PYBIND11_EMBEDDED_MODULE(Engine, m){
    m.doc() = "DeepRTS Engine";
    init_Random(m);
    init_Constants(m);
    init_BaseState(m);
    init_Unit(m);
    init_Map(m);
    init_Tile(m);
    init_Tilemap(m);
    init_Game(m);
    init_Player(m);
    init_version(m);
    init_Config(m);
    init_UnitManager(m);
    init_scenarios(m);
}
#endif