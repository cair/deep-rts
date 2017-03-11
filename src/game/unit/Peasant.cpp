//
// Created by Per-Arne on 24.02.2017.
//

#include "Peasant.h"
#include "../player/Player.h"

Peasant::Peasant(Player &player): Unit(&player) {
    typeId = Constants::Unit::Peasant;
    health = 30;
    health_max = 30;
    direction = 0;


    groundUnit = true;
    waterUnit = false;

    damageMin = 2;
    damageMax = 9;
    damageRange = 1;
    damagePiercing = 2;
    armor = 0;

    goldCarry = 0;
    lumberCarry = 0;
    oilCarry = 0;
    carryCapacity = 10;
    speed = 10;
    sight = 4;

    canHarvest = true;
    canAttack = true;
    canMove = true;
    military = false;
    structure = false;

    lumberCost = 0;
    goldCost = 400;
    oilCost = 0;

    spawnDuration = 45 * Config::getInstance().getTickModifier();

    foodProduction = 0;
    foodConsumption = 1;

    width = 1;
    height = 1;

    name = "Peasant";

    buildInventory = player_->inventoryManager.peasant;

}
