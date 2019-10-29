#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "../../cplusplus/src/player/Player.h"
#include "../../cplusplus/src/unit/Unit.h"
#include "../../cplusplus/src/Game.h"


void init_Player(py::module &m) {
    py::class_<Player>(m, "Player")
            .def(py::init<Game&, int>())

            .def_readonly("statistic_gathered_gold", &Player::sGatheredGold)
            .def_readonly("statistic_gathered_lumber", &Player::sGatheredLumber)
            .def_readonly("statistic_gathered_oil", &Player::sGatheredOil)
            .def_readonly("statistic_damage_done", &Player::sDamageDone)
            .def_readonly("statistic_damage_taken", &Player::sDamageTaken)
            .def_readonly("statistic_unit_created", &Player::sUnitsCreated)


            .def("oil", &Player::getOil)
            .def("gold", &Player::getGold)
            .def("lumber", &Player::getLumber)
            .def("foodConsumption", &Player::getFoodConsumption)
            .def("food", &Player::getFood)

            .def("get_name", &Player::getName)
            .def("set_name", &Player::setName)

            .def("is_defeated", &Player::isDefeated)


            .def("get_id", &Player::getId)
            .def("do_action", &Player::do_action)
            .def("do_manual_action", &Player::do_manual_action)


            .def("right_click", (void (Player::*)(int, int)) &Player::rightClick)
            .def("get_targeted_unit", &Player::getTargetedUnit, py::return_value_policy::reference)
            .def("left_click", &Player::leftClick);
            //.def("right_click", &Player::rightClick);

}