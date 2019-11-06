#include <pybind11/pybind11.h>
namespace py = pybind11;

#include "../src/player/Player.h"
#include "../src/unit/UnitManager.h"



void init_UnitManager(py::module &m) {
    py::class_<UnitManager>(m, "UnitManager")
        // static Unit constructUnit(Constants::Unit unitType, Player &player);
        .def("construct_unit", &UnitManager::constructUnit);

}
