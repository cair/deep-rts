#include <pybind11/pybind11.h>
namespace py = pybind11;

#include <iostream>
#include "../include/DeepRTS/Tile.h"
#include "../include/DeepRTS/Tilemap.h"
#include "unit/Unit.h"
using DeepRTS::Tile;
using DeepRTS::Tilemap;
void init_Tile(py::module &m) {
    py::class_<Tile>(m, "Tile")
            .def(py::init<Tilemap &, int, int, int, int, int, std::string &, int, bool, bool, bool, int, std::string &, int, bool, bool, bool, int, int, int, int, double, double>())
            .def_readonly("id", &Tile::id)
            .def_readonly("stone_yield", &Tile::stoneYield)
            .def_readonly("lumber_yield", &Tile::lumberYield)
            .def_readonly("gold_yield", &Tile::goldYield)
            .def_readonly("x", &Tile::x)
            .def_readonly("y", &Tile::y)
            .def_readonly("height", &Tile::height)
            .def_readonly("width", &Tile::width)


            .def("is_walkable", &Tile::isWalkable)
            .def("is_harvestable", &Tile::isHarvestable)
            .def("is_buildable", &Tile::isBuildable)
            .def("is_attackable", &Tile::isAttackable)
            .def("get_occupant", &Tile::getOccupant) // TODO - does not work
            .def("get_occupant_id", &Tile::getOccupantID)
            .def("has_occupant", &Tile::hasOccupant)
            .def("get_resources", &Tile::getResources)
            .def("get_type_id", &Tile::getTypeId)
            .def("is_depleted", &Tile::isDepleted)
            .def("get_name", &Tile::getName);

}