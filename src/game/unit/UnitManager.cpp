//
// Created by Per-Arne on 24.02.2017.
//

#include "UnitManager.h"

void UnitManager::constructTownHall(Unit &u) {

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

	u.spawnDuration = 1; //255 * Config::getInstance().getTickModifier();

	u.foodProduction = 1;
	u.foodConsumption = 0;

	u.width = 3;
	u.height = 3;

	u.name = "Town-Hall";

	u.buildInventory = { Constants::Unit::Peasant };


}

void UnitManager::constructPeasant(Unit & u)
{
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
	u.sight = 4;

	u.canHarvest = true;
	u.canAttack = true;
	u.canMove = true;
	u.military = false;
	u.structure = false;

	u.lumberCost = 0;
	u.goldCost = 400;
	u.oilCost = 0;

	u.spawnDuration = 45 * Config::getInstance().getTickModifier();

	u.foodProduction = 0;
	u.foodConsumption = 1;

	u.width = 1;
	u.height = 1;

	u.name = "Peasant";

	u.buildInventory = { Constants::Unit::TownHall };

}
