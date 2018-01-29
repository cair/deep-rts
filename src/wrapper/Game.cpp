
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;
#include "../Game.h"


void init_Game(py::module &m) {
    py::class_<Game>(m, "Game")
            .def(py::init<>())


            // Functions
            .def("tick", &Game::tick)
            .def("update", &Game::update)
            .def("render", &Game::render)
            .def("caption", &Game::caption)
            .def("start", &Game::start)
            .def("stop", &Game::stop)
            .def("reset", &Game::reset)
            .def("is_terminal", &Game::isTerminal)
            .def("add_player", &Game::addPlayer)
            .def_static("get_game", &Game::getGame)


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
            .def("get_state", &Game::getState)
            .def("get_tilemap",  &Game::getMap)

            /// Setters
            .def("set_max_fps", &Game::setMaxFPS)
            .def("set_max_ups", &Game::setMaxUPS);
}