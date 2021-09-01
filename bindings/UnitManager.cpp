#include <pybind11/pybind11.h>
namespace py = pybind11;

#include "../include/DeepRTS/Player.h"
#include "../include/DeepRTS/UnitManager.h"



void init_UnitManager(py::module &m) {
    py::class_<UnitManager>(m, "UnitManager")
        // static Unit constructUnit(Constants::Unit unitType, Player &player);
        .def("construct_unit", &UnitManager::constructUnit);

}
