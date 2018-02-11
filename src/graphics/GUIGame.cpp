//
// Created by per on 11.02.18.
//

#include "GUIGame.h"

GUIGame::GUIGame(): gui(*this){

}

void GUIGame::_render() {
    gui.render();
}
