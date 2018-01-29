#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include <iostream>
#include "../environment/Tilemap.h"
#include "../Game.h"

void init_Tilemap(py::module &m) {
    py::class_<Tilemap>(m, "Tilemap")
            .def(py::init<std::string&, Game&>())



            .def_readonly("tile_width", &Tilemap::TILE_WIDTH)
            .def_readonly("tile_height", &Tilemap::TILE_HEIGHT)
            .def_readonly("map_width", &Tilemap::MAP_WIDTH)
            .def_readonly("map_height", &Tilemap::MAP_HEIGHT)

            .def("get_tiles", &Tilemap::getTiles)
            .def("get_tile", &Tilemap::getTile);
}