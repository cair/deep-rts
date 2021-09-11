#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include "../include/DeepRTS/Map.h"
#include "../include/DeepRTS/Tilemap.h"
#include "../include/DeepRTS/Game.h"

using DeepRTS::Tilemap;
using DeepRTS::Game;
using DeepRTS::Map;
void init_Tilemap(py::module &m) {
    py::class_<Tilemap>(m, "Tilemap")
            .def(py::init<Map&, Game&>())
            .def_readonly("tiles", &Tilemap::tiles)
            .def("get_tile", &Tilemap::getTile, py::return_value_policy::reference);
}