#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;


#include "../src/unit/Unit.h"
#include "../src/player/Player.h"
#include "../src/environment/Tile.h"

void init_Unit(py::module &m) {
    py::class_<Unit>(m, "Unit")
            .def(py::init<Player&>())

            .def_readonly("id", &Unit::id)
            .def_readonly("name_id", &Unit::nameID)
            .def_readonly("type", &Unit::typeId)
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

            .def_readonly("structure", &Unit::structure)
            .def_readonly("can_attack", &Unit::canAttack)
            .def_readonly("can_move", &Unit::canMove)
            .def_readonly("military", &Unit::military)
            .def_readonly("recallable", &Unit::recallable)


            .def_readonly("sight", &Unit::sight)
            .def_readonly("speed", &Unit::speed)
            .def_readonly("armor", &Unit::armor)
            .def_readonly("damage_min", &Unit::damageMin)
            .def_readonly("damage_max", &Unit::damageMax)
            .def_readonly("damage_range", &Unit::damageRange)
            .def_readonly("damage_piercing", &Unit::damagePiercing)
            .def_readonly("direction", &Unit::direction)

            .def_readonly("walking_timer", &Unit::walking_timer)
            .def_readonly("walking_interval", &Unit::walking_interval)
            .def_readwrite("walking_path", &Unit::walking_path)

            .def_readonly("harvest_timer", &Unit::harvestTimer)
            .def_readonly("harvest_interval", &Unit::harvestInterval)

            .def_readonly("combat_timer", &Unit::combatTimer)
            .def_readonly("combat_interval", &Unit::combatInterval)


            .def_readonly("spawn_duration", &Unit::spawnDuration)
            .def_readonly("spawn_timer", &Unit::spawnTimer)
            .def_readonly("build_timer", &Unit::buildTimer)
            .def_readonly("tile", &Unit::tile)
            .def_readonly("state", &Unit::state)
            .def("get_next_tile", &Unit::getNextTile)

            .def("get_player", &Unit::getPlayer, py::return_value_policy::reference)
            .def("right_click", &Unit::rightClick)
            .def("build", &Unit::build)
            .def_readwrite("animation_counter", &Unit::animationCounter)


            ;

}
