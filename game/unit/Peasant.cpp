//
// Created by Per-Arne on 24.02.2017.
//

#include "Peasant.h"

Peasant::Peasant(Player &player): Unit(player) {
    type_id = Constants::Unit_Peasant;
    health = 30;
    health_max = 30;
    direction = 0;


    groundUnit = true;
    waterUnit = false;

    damage_min = 2;
    damage_max = 9;
    damage_range = 1;
    damage_piercing = 2;
    armor = 0;

    goldCarry = 0;
    lumberCarry = 0;
    oilCarry = 0;
    harvestInventory = 0;
    harvestCapacity = 10;
    speed = 10;
    sight = 4;

    canHarvest = true;
    canAttack = true;
    canMove = true;
    military = false;
    structure = false;

    lumberCost = 0;
    goldCost = 400;

    spawnDuration = 45 * Config::getInstance().getTickModifier();

    foodProduction = 0;
    foodConsumption = 1;

    width = 1;
    height = 1;

    name = "Peasant";

}
