//
// Created by Per-Arne on 24.02.2017.
//

#include "Unit.h"
#include "../player/Player.h"
#include "../Game.h"
#include "../lib/Pathfinder.h"
#include "../graphics/GUI.h"
#include <random>

int Unit::gId = 0;
Unit::Unit(Player &player): player_(player), stateManager(player.game_.stateManager){


    id = Unit::gId++;
    state = &stateManager.despawnedState;
    current_state = state->id;

    testTexture = new sf::Texture();
    testTexture->loadFromFile("data/textures/human/peasant.png");

    testSprite = new sf::Sprite();
    testSprite->setTexture(*testTexture);

    //testSprite->setColor(sf::Color(255, 0, 0, 200));
    testSprite->setTextureRect(sf::IntRect(10,0, 40,40));


}
void Unit::spawn(Tile &_toSpawnOn, int initValue) {
    spawnTimer = initValue;
    spawnTile = &_toSpawnOn;
    transitionState(stateManager.spawnState);
    enqueueState(stateManager.idleState);
}

void Unit::move(Tile &targetTile){
    if (!canMove)
        return;

    this->walkingGoal = &targetTile;

    transitionState(stateManager.walkingState);
}


void Unit::setPosition(Tile &newTile) {

    if(tile){                       // If unit already stands on a tile
        clearTiles();
    }

    for(auto &t : player_.game_.map.getTiles(&newTile, width, height)) {
        t->setOccupant(*this);
    }


    newTile.setOccupant(*this);         // Set occupant of new tile to this
    tile = &newTile;                     // Set this units tile to new tile
    testSprite->setPosition(tile->getPixelPosition());
}

void Unit::update() {
    state->update(*this);

}


bool Unit::build(int idx) {
    if(state->id != Constants::State_Idle)
        return false;

    if((idx < 0 or idx >= buildInventory.size()))
        return false;

    Unit &newUnit = *buildInventory[idx];
    if(!player_.canAfford(&newUnit)) {
        std::cout << "Cannot afford " << newUnit.name << std::endl;
        return false;
    }

    if(player_.canPlace(&newUnit, tile)) {

        Unit *unit = new Unit(newUnit);

        player_.addUnit(unit);


        if(!structure and unit->structure) {
           // *this is a unit (peasant), which builds a building
            Tile *spawnTile = tile;
            assert(spawnTile);
            despawn();

            buildEntity = unit;
            transitionState(stateManager.buildingState);
            buildEntity->spawn(*spawnTile, 0);
            buildEntity->setPosition(*spawnTile);


        }else if(structure and !unit->structure){
            // Structure builds unit (Ie: TownHall builds Peasant)
            buildEntity = unit;
            // build entity has no tile, spawn on nearest walkable
            Tile *firstWalkable = Pathfinder::find_first_walkable_tile(tile);
            assert(firstWalkable);
            buildEntity->spawn(*firstWalkable, 0);

            transitionState(stateManager.buildingState);
        }


        player_.subGold(unit->goldCost);
        player_.subLumber(unit->lumberCost);
        player_.subOil(unit->oilCost);

    } else {
        std::cout << "Cannot build here" << std::endl;
    }






}

void Unit::despawn() {
    if(player_.game_.gui)
        player_.game_.gui->selectedUnit = NULL;

    clearTiles();
    transitionState(stateManager.despawnedState);
}

void Unit::clearTiles(){
    for(auto &t : player_.game_.map.getTiles(tile, width, height)) {
        t->occupant = NULL;
    }
    tile->occupant = NULL;
    tile = NULL;
}

void Unit::rightClick(Tile &tile) {

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

    Unit *target = tile.occupant;
    assert(target);
    combatTarget = target;

    if(distance(tile) > 1){
        Tile* nearestWalkable = Pathfinder::find_first_walkable_tile(&tile);
        assert(nearestWalkable);

        enqueueState(stateManager.combatState);
        move(*nearestWalkable);


    }
    else {
        transitionState(stateManager.combatState);

    }



}

void Unit::harvest(Tile &tile) {
    if(!canHarvest)
        return;

    this->harvestTarget = &tile;
    transitionState(stateManager.harvestingState);

}

void Unit::enqueueState(BaseState &state) {
    stateList.push_back(&state);
}

void Unit::transitionState() {
    if(stateList.size() == 0) {
        // No states to transition to, enter idle state
        BaseState &nextState = stateManager.idleState;
        //std::cout << "State Transition: " << state->name << " ==> " << nextState.name << "|" << std::endl;
        state = &nextState;
        current_state = state->id;
        state->init(*this);
        return;
    }

    BaseState &nextState = *stateList.back();
    //std::cout << "State Transition: " << state->name << " ==> " << nextState.name << "|" << std::endl;
    state = &nextState;
    stateList.pop_back();
    current_state = state->id;
    //state->init(*this);
    return;

}

void Unit::transitionState(BaseState &nextState) {
    //std::cout << "State Transition: " << state->name << " ==> " << nextState.name << "|" << std::endl;
    state = &nextState;
    current_state = state->id;
    state->init(*this);
    return;
}

int Unit::distance(Tile &target) {
    int dim = 0; // TODO
    double d = hypot(tile->x - (target.x + dim), tile->y - (target.y + dim));
    return (int)d - dim;

}

Unit *Unit::closestRecallBuilding() {
    Unit* closest = NULL;
    int dist = INT_MAX;
    for(auto &u : player_.units) {
        if(u->recallable) {
            int d = distance(*u->tile);
            if(d < dist) {
                dist = d;
                closest = u;
            }
        }
    }
    return closest;
}

void Unit::afflictDamage(int dmg_) {
    health = std::max(0, health - dmg_);

    if (health <= 0) {
        transitionState(stateManager.deadState);

        player_.checkDefeat();
        player_.game_.checkTerminal();
    }

}

bool Unit::isDead() {
    return state->id == Constants::State_Dead;
}

int Unit::getDamage(Unit &target) {

    // TODO better random
    double output = damageMin + (rand() % (damageMax - damageMin + 1));
    double myDamage = output - target.armor + damagePiercing;

    double mini = myDamage * .50;
    double output2 = mini + (rand() % (int)(myDamage - mini + 1));

    return floor(output2);





}