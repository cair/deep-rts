//
// Created by Per-Arne on 24.02.2017.
//

#include "Walking.h"
#include "../player/Player.h"
#include "../Game.h"

void Walking::update(Unit &unit)const{

    int x = unit.walking_goal_x;
    int y = unit.walking_goal_y;
    int idx = unit.player_.game_.map.MAP_WIDTH*y + x;

    Tile &tile = unit.player_.game_.map.tiles[idx];

    unit.setPosition(tile);


}

void Walking::end(Unit &unit)const{

}

void Walking::init(Unit &unit)const{

}

