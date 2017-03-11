//
// Created by Per-Arne on 27.02.2017.
//

#ifndef WARC2SIM_INVENTORYMANAGER_H
#define WARC2SIM_INVENTORYMANAGER_H


#include "Unit.h"

class InventoryManager {


public:
    InventoryManager(Player &player);
    std::vector<Unit> townhall;
    std::vector<Unit> peasant;
};


#endif //WARC2SIM_INVENTORYMANAGER_H
