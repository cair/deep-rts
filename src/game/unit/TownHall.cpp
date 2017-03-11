//
// Created by Per-Arne on 24.02.2017.
//

#include "TownHall.h"
#include "../player/Player.h"

TownHall::TownHall(Player &player): Unit(player) {
    typeId = Constants::Unit::TownHall;
    health = 1200;
    health_max = 1200;
    direction = 0;


    groundUnit = true;
    waterUnit = false;

    damageMin = 0;
    damageMax = 0;
    damageRange = 0;
    damagePiercing = 0;
    armor = 0;

    goldCarry = 0;
    lumberCarry = 0;
    oilCarry = 0;
    carryCapacity = 0;
    speed = 0;
    sight = 4;

    canHarvest = false;
    canAttack = false;
    canMove = false;
    military = false;
    structure = true;
    recallable = true;

    lumberCost = 250;
    goldCost = 500;
    oilCost = 0;

    spawnDuration = 1; //255 * Config::getInstance().getTickModifier();

    foodProduction = 1;
    foodConsumption = 0;

    width = 3;
    height = 3;

    name = "Town-Hall";

    buildInventory = player_.inventoryManager.townhall;

}
