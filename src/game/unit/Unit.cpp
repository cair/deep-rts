//
// Created by Per-Arne on 24.02.2017.
//

#include "Unit.h"
#include "../player/Player.h"
#include "../Game.h"
#include "../util/Pathfinder.h"
#include "../graphics/GUI.h"
#include "../action/RightClickAction.h"
#include "./UnitManager.h"
#include <random>
#include "../action/BaseAction.h"

int Unit::gId = 0;
Unit::Unit(Player *player): player_(player), stateManager(&player->game_.stateManager){
    id = Unit::gId++;
    state = stateManager->despawnedState;
    current_state = state->id;


}
void Unit::spawn(Tile &_toSpawnOn, int initValue) {
    spawnTimer = initValue;
    spawnTile = &_toSpawnOn;
    transitionState(stateManager->spawnState);
	enqueueState(stateManager->idleState);
}

void Unit::moveRelative(int x, int y) {


    int newX = tile->x + x;
    int newY = tile->y + y;

    Tile *moveTile = player_->game_.map.getTile(newX, newY);

    move(*moveTile);
}

void Unit::rightClickRelative(int x, int y) {
	if (!tile) return; // Not standing on any tiles

    int newX = tile->x + x;
    int newY = tile->y + y;

    Tile *clickTile = player_->game_.map.getTile(newX, newY);
    rightClick(*clickTile);
}

void Unit::move(Tile &targetTile){
    if (!canMove)
        return;
    this->walkingGoal = &targetTile;
    transitionState(stateManager->walkingState);
}


void Unit::setPosition(Tile &newTile) {

    if(tile){                       // If unit already stands on a tile
        clearTiles();
    }

    for(auto &t : player_->game_.map.getTiles(&newTile, width, height)) {
        t->setOccupant(this);
    }


    newTile.setOccupant(this);         // Set occupant of new tile to this
    tile = &newTile;                     // Set this units tile to new tile
    sf::Vector2f newPos = tile->getPixelPosition();
    setDirection(newPos.x, newPos.y);
    worldPosition = newPos;
}

void Unit::update() {
    state->update(*this);

}

Tile *Unit::centerTile() {
	int addX = floor(width / 2);
	int addY = floor(height / 2);

	if (addX == 0 && addY == 0) {
		assert(tile);
		return tile;
	}

	return player_->game_.map.getTile(tile->x + addX, tile->y + addY);
}

bool Unit::build(int idx) {
    //if(state->id != Constants::State::Idle)
    if(current_state != Constants::State::Idle)
        return false;

    if((idx < 0 or idx >= buildInventory.size()))
        return false;

	Unit newUnit = UnitManager::constructUnit(buildInventory[idx], player_);

    // PlacementTile is based on dimension of the new unit. For example; town hall has
    // 3x Width and 3x Height. We then want to place  the building by the middle tile;
    Tile *placementTile = player_->game_.map.getTile(tile->x - floor(newUnit.width/2), tile->y - floor(newUnit.height/2));
	assert(placementTile && "PlacementTile was null in build()");
    if(!player_->canAfford(newUnit)) {
        //std::cout << "Cannot afford " << newUnit->name << std::endl;
        return false;
    }



    if(player_->canPlace(*this, newUnit, placementTile)) {

        Unit &unit = player_->addUnit(newUnit.typeId);
		unit.player_ = player_;
		unit.builtBy = this;

        if(!structure and unit.structure) {
           // *this is a unit (peasant), which builds a building
            despawn();

            buildEntity = &unit;
            transitionState(stateManager->buildingState);
            buildEntity->spawn(*placementTile, 0);
            buildEntity->setPosition(*placementTile);


        }else if(structure and !unit.structure){
            // Structure builds unit (Ie: TownHall builds Peasant)
            buildEntity = &unit;
            // build entity has no tile, spawn on nearest walkable
            Tile *firstWalkable = Pathfinder::find_first_walkable_tile(centerTile());
            assert(firstWalkable);
            buildEntity->spawn(*firstWalkable, 0);

            transitionState(stateManager->buildingState);
        }


        player_->subGold(unit.goldCost);
        player_->subLumber(unit.lumberCost);
        player_->subOil(unit.oilCost);
        player_->statUnitBuilt += 1;

    } else {
        std::cout << "Cannot build here" << std::endl;
    }






}

void Unit::despawn() {

    for(auto &p: player_->game_.players) {
        p.targetedUnit = NULL;
    }

    clearTiles();
    transitionState(stateManager->despawnedState);
}

void Unit::clearTiles(){
    for(auto &t : player_->game_.map.getTiles(tile, width, height)) {
        t->setOccupant(NULL);
    }
    tile->setOccupant(NULL);
    tile = NULL;
}

void Unit::rightClick(Tile &tile) {
	stateList.clear();
	transitionState();


    std::shared_ptr<BaseAction> action = std::shared_ptr<BaseAction>(new RightClickAction(*this));
    player_->game_.addAction(action);


    if(tile.isHarvestable()){
        //  Harvest
        harvest(tile);
    }
    else if(tile.isAttackable(*this)){
        // Attack
        attack(tile);
    }
    else if(tile.isWalkable()){
        // Walk
        move(tile);
    }
}

void Unit::attack(Tile &tile) {
    if(!canAttack)
        return;

    Unit* target = tile.getOccupant();

	// Target may have died from another same tick
	if (!target) {
		transitionState();
		return;
	}

    combatTarget = target;

    if(distance(tile) > 1){
        enqueueState(stateManager->combatState);
        move(tile);
    }
    else {
        transitionState(stateManager->combatState);

    }



}

void Unit::harvest(Tile &tile) {
    if(!canHarvest)
        return;

    this->harvestTarget = &tile;
    transitionState(stateManager->harvestingState);

}

void Unit::enqueueState(std::shared_ptr<BaseState> state) {
    stateList.push_back(state);
}

void Unit::transitionState() {
    if(stateList.empty()) {
        // No states to transition to, enter idle state
		std::shared_ptr<BaseState> nextState = stateManager->idleState;
        //std::cout << "State Transition: " << state->name << " ==> " << nextState->name << "|" << std::endl;
		//std::cout << "State Transition: " << state->id << " ==> " << nextState->id << "|" << std::endl;
		state->end(*this);
        state = nextState;
        current_state = state->id;
        state->init(*this);
        return;
    }

	std::shared_ptr<BaseState> nextState = stateList.back();
    //std::cout << "State Transition: " << state->name << " ==> " << nextState->name << "|y" << std::endl;
	//std::cout << "State Transition: " << state->id << " ==> " << nextState->id << "|y"  << std::endl;
	state->end(*this);
    state = nextState;
    stateList.pop_back();
    current_state = state->id;
    //state->init(*this);


}

void Unit::transitionState(std::shared_ptr<BaseState> nextState) {
    //std::cout << "State Transition: " << state->name << " ==> " << nextState->name << "|x" << std::endl;
	//std::cout << "State Transition: " << state->id << " ==> " << nextState->id << "|x" << std::endl;
	state->end(*this);
    state = nextState;
    current_state = state->id;
    state->init(*this);
    return;
}

int Unit::distance(Tile &target) {
    int dim = 0; // TODO
    double d = hypot(tile->x - (target.x + dim), tile->y - (target.y + dim));
    return (int)d - dim;

}


int Unit::distance(Unit & target) {
	int targ_x = target.tile->x;
	int targ_y = target.tile->y;
	int dim_x = floor(target.width / 2);
	int dim_y = floor(target.height / 2);

	double d = hypot(tile->x - (targ_x + dim_x), tile->y - (targ_y + dim_y));

	return (int)d - dim_x;

}

sf::Vector2f Unit::distanceVector(Tile &target){
    int dx = tile->x - target.x;
    int dy = tile->y - target.y;

    return sf::Vector2f(dx, dy);
}

Unit* Unit::closestRecallBuilding() {
    Unit* closest = NULL;
    int dist = INT_MAX;
    for(auto &unit : player_->game_.units) {
        if(unit.recallable && unit.player_ == player_ && unit.tile) {
            int d = distance(*unit.tile);
            if(d < dist) {
                dist = d;
                closest = &unit;
            }
        }
    }
    return closest;
}

void Unit::afflictDamage(int dmg_) {
    health = std::max(0, health - dmg_);

    if (health <= 0) {
        transitionState(stateManager->deadState);

        player_->checkDefeat();
        player_->game_.checkTerminal();
    }

}

bool Unit::isDead() {
    return state->id == Constants::State::Dead or state->id == Constants::State::Despawned;
}

int Unit::getDamage(Unit &target) {

    // TODO better random
    double output = damageMin + (rand() % (damageMax - damageMin + 1));
    double myDamage = output - target.armor + damagePiercing;

    double mini = myDamage * .50;
    double output2 = mini + (rand() % (int)(myDamage - mini + 1));

    return floor(output2);

}

void Unit::setDirection(sf::Vector2f &dir){
    setDirection(dir.x, dir.y);
}

void Unit::setDirection(int newX, int newY){
    int oldX = worldPosition.x;
    int oldY = worldPosition.y;

    int dx = (newX - oldX);
    int dy = (newY - oldY);

    if (dx > 0 and dy > 0) {
        // Down Right
        direction = Constants::Direction::DownRight;
        //std::cout << "Down Right" << std::endl;
    } else if (dx < 0 and dy > 0) {
        // Down Left
        direction = Constants::Direction::DownLeft;
        //std::cout << "Down Left" << std::endl;
    } else if (dx > 0 and dy < 0) {
        // Up Right
        direction = Constants::Direction::UpRight;
        //std::cout << "Up Right" << std::endl;
    } else if (dx < 0 and dy < 0) {
        // Up Left
        direction = Constants::Direction::UpLeft;
        //std::cout << "Up Left" << std::endl;
    } else if (dx > 0 and dy == 0) {
        // Right
        direction = Constants::Direction::Right;
        //std::cout << "Right" << std::endl;
    } else if (dx < 0 and dy == 0) {
        // Left
        direction = Constants::Direction::Left;
        //std::cout << "Left" << std::endl;
    } else if (dx == 0 and dy < 0) {
        // Up
        direction = Constants::Direction::Up;
        //std::cout << "Up" << std::endl;
    } else if (dx == 0 and dy > 0) {
        // Down
        direction = Constants::Direction::Down;
        //std::cout << "Down" << std::endl;
    }



}

bool Unit::operator==(int otherID) const
{
	return otherID == id;
}

void Unit::tryAttack()
{
	if (!tile) {
		// FAIL
		return;
	}

	std::vector<Tile *> availableAttackable = player_->game_.getMap().neighbors(*tile, Constants::Pathfinding::Attackable);
	if (availableAttackable.empty()) {
		// Fail
		return;
	}else {
		// Success
		attack(*availableAttackable.back());
	}

}

void Unit::tryMove(int16_t x, int16_t y)
{
	if (!tile) {
		// FAil
		return;
	}

	int newX = tile->x + x;
	int newY = tile->y + y;

	Tile *tile = player_->game_.map.getTile(newX, newY);
	if (tile->isWalkable()) {
		move(*tile);
	}
	else {
		// Fail
		return;
	}
}

void Unit::tryHarvest()
{
	if (!tile) {
		// FAIL
		return;
	}

	std::vector<Tile *> availableHarvestable = player_->game_.getMap().neighbors(*tile, Constants::Pathfinding::Harvestable);
	if (availableHarvestable.empty()) {
		// Fail
		return;
	}
	else {
		harvest(*availableHarvestable.back());
	}
}
