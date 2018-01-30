#include <pybind11/pybind11.h>
namespace py = pybind11;

#include "../unit/Unit.h"
#include "../player/Player.h"
#include "../environment/Tile.h"

void init_Unit(py::module &m) {
    py::class_<Unit>(m, "Unit")
            .def(py::init<Player*>())


            .def_readonly("type_id", &Unit::typeId)
            .def_readonly("health", &Unit::health)
            .def_readonly("health_max", &Unit::health_max)
            .def_readonly("name", &Unit::name)
            .def_readonly("width", &Unit::width)
            .def_readonly("height", &Unit::height)
            .def_readonly("lumber_cost", &Unit::lumberCost)
            .def_readonly("gold_cost", &Unit::goldCost)
            .def_readonly("oil_cost", &Unit::oilCost)
            .def_readonly("lumber_carry", &Unit::lumberCarry)
            .def_readonly("gold_carry", &Unit::goldCarry)
            .def_readonly("oil_carry", &Unit::oilCarry)

            .def_readonly("sight", &Unit::sight)
            .def_readonly("speed", &Unit::speed)
            .def_readonly("armor", &Unit::armor)
            .def_readonly("damage_min", &Unit::damageMin)
            .def_readonly("damage_max", &Unit::damageMax)
            .def_readonly("damage_range", &Unit::damageRange)
            .def_readonly("damage_piercing", &Unit::damagePiercing)
            .def_readonly("direction", &Unit::direction)


            .def_readonly("spawn_duration", &Unit::spawnDuration)
            .def_readonly("spawn_timer", &Unit::spawnTimer)
            .def_readonly("build_timer", &Unit::buildTimer)
            .def_readonly("tile", &Unit::tile)
            .def_readonly("state", &Unit::state)



            ;

}