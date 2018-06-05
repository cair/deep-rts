//
// Created by per on 24.02.18.
//
#include <pybind11/pybind11.h>
namespace py = pybind11;
#include "../../cplusplus/src/Config.h"

void init_Config(py::module &m) {
    py::class_<Config>(m, "Config")
            .def(py::init<>())
            .def("set_tick_modifier", &Config::setTickModifier)
            .def("set_instant_town_hall", &Config::setInstantTownHall)
            .def("set_instant_building", &Config::setInstantBuilding)
            .def("set_harvest_forever", &Config::setHarvestForever)
            .def("set_auto_attack", &Config::setAutoAttack)
            .def("set_food_limit", &Config::setFoodLimit)
            .def("set_farm", &Config::setFarm)
            .def("set_barracks", &Config::setBarracks)
            .def("set_footman", &Config::setFootman)
            .def("set_archer", &Config::setArcher)
            .def("set_audio", &Config::setAudio)
            .def_static("defaults", &Config::defaults);
}