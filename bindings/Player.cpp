#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "../src/player/Player.h"
#include "../src/unit/Unit.h"
#include "../src/Game.h"


void init_Player(py::module &m) {
    py::class_<Player>(m, "Player")
            .def(py::init<Game&, int>())

            .def_readonly("statistic_gathered_gold", &Player::sGatheredGold)
            .def_readonly("statistic_gathered_lumber", &Player::sGatheredLumber)
            .def_readonly("statistic_gathered_oil", &Player::sGatheredOil)
            .def_readonly("statistic_damage_done", &Player::sDamageDone)
            .def_readonly("statistic_damage_taken", &Player::sDamageTaken)
            .def_readonly("statistic_units_created", &Player::sUnitsCreated)
            .def_readonly("num_archer", &Player::num_archer)
            .def_readonly("num_barrack", &Player::num_barrack)
            .def_readonly("num_farm", &Player::num_farm)
            .def_readonly("num_footman", &Player::num_footman)
            .def_readonly("num_peasant", &Player::num_peasant)
            .def_readonly("num_town_hall", &Player::num_town_hall)

            .def_readonly("oil", &Player::oil)
            .def_readonly("gold", &Player::gold)
            .def_readonly("lumber", &Player::lumber)
            .def_readonly("food_consumption", &Player::foodConsumption)
            .def_readonly("food", &Player::food)


            .def("get_name", &Player::getName)
            .def("set_name", &Player::setName)

            .def("is_defeated", &Player::isDefeated)


            .def("get_id", &Player::getId)
            .def("do_action", &Player::do_action)
            .def("do_manual_action", &Player::do_manual_action)

            .def("set_targeted_unit_id", &Player::setTargetedUnitID)

            .def("right_click", (void (Player::*)(int, int)) &Player::rightClick)
            .def("get_targeted_unit", &Player::getTargetedUnit, py::return_value_policy::reference)
            .def("left_click", &Player::leftClick);

            //.def("right_click", &Player::rightClick);

}
