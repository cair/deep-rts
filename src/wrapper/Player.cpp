#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "../player/Player.h"
#include "../Game.h"


void init_Player(py::module &m) {
    py::class_<Player>(m, "Player")
            .def(py::init<Game&, int>())

            .def_readonly("sGatheredGold", &Player::sGatheredGold)
            .def_readonly("sGatheredLumber", &Player::sGatheredGold)
            .def_readonly("sGatheredOil", &Player::sGatheredGold)
            .def_readonly("sDamageDone", &Player::sGatheredGold)
            .def_readonly("sDamageTaken", &Player::sGatheredGold)
            .def_readonly("sUnitsCreated", &Player::sGatheredGold)

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
            .def("do_manual_action", &Player::do_manual_action);
}