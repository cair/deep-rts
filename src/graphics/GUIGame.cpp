//
// Created by per on 11.02.18.
//

#include "GUIGame.h"

GUIGame::GUIGame(std::string map_file) : Game(map_file), gui(*this) {

}

void GUIGame::_render() {
    gui.render();
}
