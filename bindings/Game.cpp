
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../src/util/pybind11_matrix.h"
namespace py = pybind11;
#include "./trampolines/PyGame.h"

void init_Game(py::module &m) {
    py::class_<Game, PyGame>(m, "Game")
            .def(py::init<std::string>())
            .def(py::init<std::string, Config>())

                    // Functions
            .def_readonly("config", &Game::config)
            .def("tick", &Game::tick)
            .def("update", &Game::update)
            .def("_update", &Game::update)
            .def("render", &Game::render)
            .def("_render", &Game::render)

            .def("caption", &Game::caption)
            .def("_caption", &Game::caption)
            .def("start", &Game::start)
            .def("stop", &Game::stop)
            .def("reset", &Game::reset)
            .def("is_terminal", &Game::isTerminal)
            .def("add_player", &Game::addPlayer, py::return_value_policy::reference)
            .def_static("get_game", &Game::getGame, py::return_value_policy::reference)

                    // Callbacks
            .def("_on_unit_create", &Game::_onUnitCreate)
            .def("_on_unit_destroy", &Game::_onUnitDestroy)
            .def("_on_episode_start", &Game::_onEpisodeStart)
            .def("_on_episode_end", &Game::_onEpisodeEnd)
            .def("_on_tile_change", &Game::_onTileChange)
            .def("_on_resource_gather", &Game::_onResourceGather)
            .def("_on_resource_depleted", &Game::_onResourceDepleted)

            /// Getters
            .def("get_id", &Game::getId)
            .def("get_episode", &Game::getEpisode)
            .def("get_height", &Game::getHeight)
            .def("get_width", &Game::getWidth)
            .def("get_ticks", &Game::getTicks)
            .def("get_episode_duration", &Game::getGameDuration)
            .def("get_fps", &Game::getFPS)
            .def("get_ups", &Game::getUPS)
            .def("get_max_fps", &Game::getMaxFPS)
            .def("get_max_ups", &Game::getMaxUPS)
            .def("get_ticks_modifier", &Game::getTicksModifier)

            .def("get_unit_by_name_id", &Game::getUnitByNameID, py::return_value_policy::reference)

                    /// Setters
            .def("set_max_fps", &Game::setMaxFPS)
            .def("set_max_ups", &Game::setMaxUPS)
            .def("set_selected_player", &Game::setSelectedPlayer)


            .def_readonly("state", &Game::state)

            .def_readonly("units", &Game::units)
            .def_readonly("players", &Game::players)
            .def_readonly("tilemap", &Game::tilemap)
            .def_readonly("map", &Game::map)
            .def_readonly("selected_player", &Game::selectedPlayer);




}
