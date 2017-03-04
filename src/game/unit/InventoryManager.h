//
// Created by Per-Arne on 27.02.2017.
//

#ifndef WARC2SIM_INVENTORYMANAGER_H
#define WARC2SIM_INVENTORYMANAGER_H


#include "Unit.h"

class InventoryManager {


public:
    InventoryManager(Player &player);
    std::vector<std::shared_ptr<Unit>> townhall;
    std::vector<std::shared_ptr<Unit>> peasant;
};


#endif //WARC2SIM_INVENTORYMANAGER_H
