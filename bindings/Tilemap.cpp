#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include "../src/environment/Map.h"
#include "../src/environment/Tilemap.h"
#include "../src/Game.h"

void init_Tilemap(py::module &m) {
    py::class_<Tilemap>(m, "Tilemap")
            .def(py::init<Map&, Game&>())
            .def_readonly("tiles", &Tilemap::tiles)
            .def("get_tile", &Tilemap::getTile, py::return_value_policy::reference);
}