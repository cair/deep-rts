//
// Created by per on 23.03.18.
//

#include "../../include/DeepRTS/gamestate/AddUnit.h"
#include "../../include/DeepRTS/Unit.h"
#include "../../include/DeepRTS/Game.h"

void AddUnit::apply(Game * game){
    auto &player = game->players.at(player_id);
    player.addUnit((Constants::Unit)type);
}

void AddUnit::apply_reverse(Game* game){
    auto &player = game->players.at(player_id);
    auto &unit = game->getUnit(id);
    player.removeUnit(unit);
}
