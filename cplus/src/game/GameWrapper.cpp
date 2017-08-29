//
// Created by per on 27.08.17.
//

#include "GameWrapper.h"
#include "Game.h"


void GameWrapper::initGUI() {
    this->game->initGUI();
}

void GameWrapper::start() {
    this->game->start();
}

void GameWrapper::loop() {
    this->game->loop();
}

GameWrapper::GameWrapper() {
    this->game = new Game(4, true);
}


#include <boost/python.hpp>
BOOST_PYTHON_MODULE(DeepRTS)
{
    boost::python::class_<GameWrapper>("GameWrapper", boost::python::init<>())
            //.def("addPlayer", &Game::addPlayer)
            .def("start", &Game::start)
            .def("initGUI", &Game::initGUI)
            .def("loop", &Game::loop);


    boost::python::class_<Player>("Player", boost::python::init<Player>());
}