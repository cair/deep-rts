//
// Created by per on 04.02.18.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include <string>
#include "../src/environment/Map.h"


void init_Map(py::module &m) {
    py::class_<Map>(m, "Map")
            .def(py::init<const std::string&>())
            .def_readonly("tile_width", &Map::TILE_WIDTH)
            .def_readonly("tile_height", &Map::TILE_HEIGHT)
            .def_readonly("map_width", &Map::MAP_WIDTH)
            .def_readonly("map_height", &Map::MAP_HEIGHT);
}