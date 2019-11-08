//
// Created by per on 24.02.18.
//
#include <pybind11/pybind11.h>
namespace py = pybind11;
#include "../src/Config.h"

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
            .def("set_console_caption_enabled", &Config::setConsoleCaptionEnabled)
            .def("set_start_gold", &Config::setStartGold)
            .def("set_start_lumber", &Config::setStartLumber)
            .def("set_start_oil", &Config::setStartOil)
            .def("set_terminal_signal", &Config::setTerminalSignal)
            .def("set_pomdp", &Config::setPOMDP)


            // Properties
            .def_readonly("tick_modifier", &Config::tickModifier)
            .def_readonly("town_hall", &Config::instantTownHall)
            .def_readonly("instant_building", &Config::instantBuilding)
            .def_readonly("harvest_forever", &Config::harvestForever)
            .def_readonly("auto_attack", &Config::autoAttack)
            .def_readonly("food_limit", &Config::foodLimit)
            .def_readonly("farm_enabled", &Config::farmEnabled)
            .def_readonly("barracks_enabled", &Config::barracksEnabled)
            .def_readonly("footman_enabled", &Config::footmanEnabled)
            .def_readonly("archer_enabled", &Config::archerEnabled)
            .def_readonly("pomdp", &Config::pompd)
            .def_readonly("start_gold", &Config::startGold)
            .def_readonly("start_oil", &Config::startOil)
            .def_readonly("start_lumber", &Config::startLumber)
            .def_readonly("terminal_signal", &Config::terminalSignal)
            .def_readonly("console_caption_enabled", &Config::consoleCaptionEnabled)



            .def_static("defaults", &Config::defaults);
}
