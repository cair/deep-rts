//
// Created by Per-Arne on 24.02.2017.
//

#include "UnitManager.h"
#include "../Game.h"

Unit UnitManager::constructUnit(Constants::Unit unitType, Player& player)
{
    switch (unitType) {
        case Constants::Unit::TownHall:
            return constructTownHall(player);
        case Constants::Unit::Peasant:
            return constructPeasant(player);
        case Constants::Unit::Farm:
            return constructFarm(player);
        case Constants::Unit::Barracks:
            return constructBarracks(player);
        case Constants::Unit::Footman:
            return constructFootman(player);
        case Constants::Unit::Archer:
            return constructArcher(player);
        case Constants::Unit::Peon:
            //throw std::runtime_error("Not implemented");
            return constructPeasant(player);
        case Constants::Unit::None:
            //throw std::runtime_error("Should never happen");
            return constructPeasant(player); // TODO here! ? Dont think it breaks anything, but eeew
            break;
        default:
            throw std::runtime_error("Should never happen");
    }
}



void UnitManager::updateUnitCount(Player &p, Constants::Unit unitType, int n)
{
    switch (unitType) {
        case Constants::Unit::TownHall:
            p.num_town_hall += n;
            break;
        case Constants::Unit::Peasant:
            p.num_peasant += n;
            break;
        case Constants::Unit::Farm:
            p.num_farm += n;
            break;
        case Constants::Unit::Barracks:
            p.num_barrack += n;
            break;
        case Constants::Unit::Footman:
            p.num_footman += n;
            break;
        case Constants::Unit::Archer:
            p.num_archer += n;
            break;
        case Constants::Unit::Peon:
            throw std::runtime_error("Not implemented");
        case Constants::Unit::None:
            throw std::runtime_error("Should never happen");
        default:
            throw std::runtime_error("Should never happen");

    }
}


Unit UnitManager::constructFarm(Player &player) {
    Unit u = Unit(player);
    u.typeId = Constants::Unit::Farm;
    u.health = 400;
    u.health_max = 400;
    u.direction = 0;


    u.groundUnit = true;
    u.waterUnit = false;

    u.damageMin = 0;
    u.damageMax = 0;
    u.damageRange = 0;
    u.damagePiercing = 0;
    u.armor = 20;

    u.goldCarry = 0;
    u.lumberCarry = 0;
    u.oilCarry = 0;
    u.carryCapacity = 0;
    u.speed = 0;
    u.sight = 2;


    u.canHarvest = false;
    u.canAttack = false;
    u.canMove = false;
    u.military = false;
    u.structure = true;
    u.recallable = false;

    u.lumberCost = 250;
    u.goldCost = 500;
    u.oilCost = 0;

    if(u.config.instantBuilding) {
        u.spawnDuration = 0;
    } else {
        u.spawnDuration = 100 * u.config.tickModifier;
    }

    u.foodProduction = 4;
    u.foodConsumption = 0;

    u.width = 1;
    u.height = 1;

    u.name = "Farm";
    u.nameID = u.name + std::to_string(u.id);

    u.buildInventory = { };

    return u;
}

Unit UnitManager::constructBarracks(Player &player) {
    Unit u = Unit(player);
    u.typeId = Constants::Unit::Barracks;
    u.health = 800;
    u.health_max = 800;
    u.direction = 0;


    u.groundUnit = true;
    u.waterUnit = false;

    u.damageMin = 0;
    u.damageMax = 0;
    u.damageRange = 0;
    u.damagePiercing = 0;
    u.armor = 20;

    u.goldCarry = 0;
    u.lumberCarry = 0;
    u.oilCarry = 0;
    u.carryCapacity = 0;
    u.speed = 0;
    u.sight = 3;


    u.canHarvest = false;
    u.canAttack = false;
    u.canMove = false;
    u.military = true;
    u.structure = true;
    u.recallable = false;

    u.lumberCost = 450;
    u.goldCost = 700;
    u.oilCost = 0;

    if(u.config.instantBuilding) {
        u.spawnDuration = 0;
    } else {
        u.spawnDuration = 100 * u.config.tickModifier;
    }


    u.foodProduction = 0;
    u.foodConsumption = 0;

    u.width = 3;
    u.height = 3;

    u.name = "Barracks";
    u.nameID = u.name + std::to_string(u.id);

    u.buildInventory = {};

    if(u.config.footmanEnabled) {
        u.buildInventory.push_back(Constants::Unit::Footman);
    }
    if(u.config.archerEnabled) {
        u.buildInventory.push_back(Constants::Unit::Archer);
    }

    return u;
}

Unit UnitManager::constructTownHall(Player &player) {
    Unit u = Unit(player);
    u.typeId = Constants::Unit::TownHall;
    u.health = 1200;
    u.health_max = 1200;
    u.direction = 0;


    u.groundUnit = true;
    u.waterUnit = false;

    u.damageMin = 0;
    u.damageMax = 0;
    u.damageRange = 0;
    u.damagePiercing = 0;
    u.armor = 0;

    u.goldCarry = 0;
    u.lumberCarry = 0;
    u.oilCarry = 0;
    u.carryCapacity = 0;
    u.speed = 0;
    u.sight = 4;


    u.canHarvest = false;
    u.canAttack = false;
    u.canMove = false;
    u.military = false;
    u.structure = true;
    u.recallable = true;

    u.lumberCost = 250;
    u.goldCost = 500;
    u.oilCost = 0;

    if(u.config.instantBuilding) {
        u.spawnDuration = 0;
    } else {
        u.spawnDuration = 60 * u.config.tickModifier;
    }

    u.foodProduction = 1;
    u.foodConsumption = 0;

    u.width = 3;
    u.height = 3;

    u.name = "Town-Hall";
    u.nameID = u.name + std::to_string(u.id);

    u.buildInventory = { Constants::Unit::Peasant };

    return u;
}

Unit UnitManager::constructPeasant(Player &player)
{
    Unit u = Unit(player);

    u.typeId = Constants::Unit::Peasant;
    u.health = 30;
    u.health_max = 30;
    u.direction = 0;


    u.groundUnit = true;
    u.waterUnit = false;

    u.damageMin = 2;
    u.damageMax = 9;
    u.damageRange = 1;
    u.damagePiercing = 2;
    u.armor = 0;

    u.goldCarry = 0;
    u.lumberCarry = 0;
    u.oilCarry = 0;
    u.carryCapacity = 10;
    u.speed = 10;
    u.sight = 2;


    u.canHarvest = true;
    u.canAttack = true;
    u.canMove = true;
    u.military = false;
    u.structure = false;

    u.lumberCost = 0;
    u.goldCost = 400;
    u.oilCost = 0;

    if(u.config.instantBuilding) {
        u.spawnDuration = 0;
    } else {
        u.spawnDuration = 45 * u.config.tickModifier;
    }


    u.foodProduction = 0;
    u.foodConsumption = 1;

    u.width = 1;
    u.height = 1;

    u.name = "Peasant";
    u.nameID = u.name + std::to_string(u.id);

    u.buildInventory = { Constants::Unit::TownHall};

    if(u.config.farmEnabled) {
        u.buildInventory.push_back(Constants::Unit::Farm);
    }
    if(u.config.barracksEnabled) {
        u.buildInventory.push_back(Constants::Unit::Barracks);
    }


    return u;
}

Unit UnitManager::constructFootman(Player &player) {
    Unit u = Unit(player);

    u.typeId = Constants::Unit::Footman;
    u.health = 60;
    u.health_max = 60;
    u.direction = 0;


    u.groundUnit = true;
    u.waterUnit = false;

    u.damageMin = 2;
    u.damageMax = 9;
    u.damageRange = 1;
    u.damagePiercing = 4;
    u.armor = 4; // 2 + 4

    u.goldCarry = 0;
    u.lumberCarry = 0;
    u.oilCarry = 0;
    u.carryCapacity = 0;
    u.speed = 10;
    u.sight = 3;


    u.canHarvest = false;
    u.canAttack = true;
    u.canMove = true;
    u.military = true;
    u.structure = false;

    u.lumberCost = 0;
    u.goldCost = 600;
    u.oilCost = 0;

    if(u.config.instantBuilding) {
        u.spawnDuration = 0;
    } else {
        u.spawnDuration = 60 * u.config.tickModifier;
    }

    u.foodProduction = 0;
    u.foodConsumption = 1;

    u.width = 1;
    u.height = 1;

    u.name = "Footman";
    u.nameID = u.name + std::to_string(u.id);

    u.buildInventory = {};

    return u;
}

Unit UnitManager::constructArcher(Player &player) {
    Unit u = Unit(player);

    u.typeId = Constants::Unit::Archer;
    u.health = 40; // + 10
    u.health_max = 40;
    u.direction = 0;


    u.groundUnit = true;
    u.waterUnit = false;

    u.damageMin = 3;
    u.damageMax = 9;
    u.damageRange = 4;
    u.damagePiercing = 2;
    u.armor = 0;

    u.goldCarry = 0;
    u.lumberCarry = 0;
    u.oilCarry = 0;
    u.carryCapacity = 0;
    u.speed = 10;
    u.sight = 4; // +4


    u.canHarvest = false;
    u.canAttack = true;
    u.canMove = true;
    u.military = true;
    u.structure = false;

    u.lumberCost = 50;
    u.goldCost = 500;
    u.oilCost = 0;

    if(u.config.instantBuilding) {
        u.spawnDuration = 0;
    } else {
        u.spawnDuration = 75 * u.config.tickModifier;
    }


    u.foodProduction = 0;
    u.foodConsumption = 1;

    u.width = 1;
    u.height = 1;

    u.name = "Archer";
    u.nameID = u.name + std::to_string(u.id);

    u.buildInventory = {};

    return u;
}
