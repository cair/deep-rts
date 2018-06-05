//
// Created by per on 11.02.18.
//

#include "GUIGame.h"

GUIGame::GUIGame(std::string map_file) : Game(map_file), gui(*this) {

}

GUIGame::GUIGame(std::string map_file, Config config) : Game(map_file, config), gui(*this) {

}


void GUIGame::_render() {
    gui.render();
}

