//
// Created by Per-Arne on 24.02.2017.
//

#include "Unit.h"
#include "../Config.h"
#include "../player/Player.h"
#include "../Game.h"


int Unit::gId = 0;
Unit::Unit(Player &player): player_(player){


    id = Unit::gId++;
    state = &stateManager.despawnedState;
    current_state = state->id;
    std::cout << "Player " << player_.getId() << std::endl;

    testTexture = new sf::Texture();
    testTexture->loadFromFile("data/textures/human/peasant.png");

    testSprite = new sf::Sprite();
    testSprite->setTexture(*testTexture);
    testSprite->setTextureRect(sf::IntRect(10,0, 40,40));


}
void Unit::spawn(Tile &spawnTile) {
    transitionState(stateManager.spawnState);
    enqueueState(stateManager.idleState);
    setPosition(spawnTile);
}

void Unit::move(Tile &targetTile){

    walking_goal_x = targetTile.x;
    walking_goal_y = targetTile.y;

    BaseState &state = stateManager.walkingState;
    transitionState(state);
}


void Unit::setPosition(Tile &newTile) {

    if(tile != nullptr){              // If unit already stands on a tile
        tile->occupant = NULL;     // Set current's tile to nullptr
    }

    newTile.setOccupant(*this);         // Set occupant of new tile to this
    tile = &newTile;                     // Set this units tile to new tile

    testSprite->setPosition(tile->getPixelPosition());
}

void Unit::update() {
    state->update(*this);

}

bool Unit::build(int idx) {

}

void Unit::despawn() {

}

void Unit::rightClick(Tile &tile) {

    std::cout << "Player " << player_.getId() << std::endl;

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

}

void Unit::harvest(Tile &tile) {

}

void Unit::enqueueState(BaseState &state) {
    stateList.push_back(&state);
}

void Unit::transitionState() {
    if(stateList.size() == 0) {
        // No states to transition to, enter idle state
        BaseState &nextState = stateManager.idleState;
        std::cout << "State Transition: " << state->name << " ==> " << nextState.name << "|" << std::endl;
        state = &nextState;
        current_state = state->id;
        state->init(*this);
        return;
    }

    BaseState &nextState = *stateList.back();
    std::cout << "State Transition: " << state->name << " ==> " << nextState.name << "|" << std::endl;
    state = &nextState;
    stateList.pop_back();
    current_state = state->id;
    state->init(*this);
    return;

}

void Unit::transitionState(BaseState &nextState) {
    std::cout << "State Transition: " << state->name << " ==> " << nextState.name << "|" << std::endl;
    state = &nextState;
    current_state = state->id;
    state->init(*this);
    return;
}


