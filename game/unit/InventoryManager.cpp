//
// Created by Per-Arne on 27.02.2017.
//

#include "InventoryManager.h"
#include "Peasant.h"
#include "TownHall.h"

InventoryManager::InventoryManager(Player &player) {

    townhall.push_back(std::shared_ptr<Unit>(new Peasant(player)));
    peasant.push_back(std::shared_ptr<Unit>(new TownHall(player)));


}
