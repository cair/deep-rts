//
// Created by Per-Arne on 27.02.2017.
//

#include "Combat.h"
#include "../unit/unit.h"
#include "../player/Player.h"
#include "../util/Pathfinder.h"


void Combat::update(std::shared_ptr<Unit> unit)const{

    unit->combatTimer += 1;
    if(unit->combatTimer >= unit->combatInterval) {

        if(unit->distance(*unit->combatTarget->tile) > unit->damageRange) {
            // Too far away, Walk

			unit->walkingGoal = unit->combatTarget->tile;
			unit->transitionState(unit->stateManager.walkingState);
            unit->enqueueState(unit->stateManager.combatState);

        } else {
            // Can attack
            int myDamage = unit->getDamage(*unit->combatTarget);
            unit->combatTarget->afflictDamage(myDamage);
            unit->player_.statUnitDamageDone += myDamage;
            unit->combatTarget->player_.statUnitDamageTaken += myDamage;
            unit->combatTimer = 0;

            if(unit->combatTarget->isDead()){
                unit->combatTarget = NULL;
                unit->combatTimer = 1000;
                unit->transitionState();
                return;
            }

            if(unit->combatTarget->state->id == Constants::State_Idle) {
                unit->combatTarget->attack(*unit->tile);
            }

        }


    }

}

void Combat::end(std::shared_ptr<Unit> unit)const{

}

void Combat::init(std::shared_ptr<Unit> unit)const{
    sf::Vector2f dir = unit->distanceVector(*unit->combatTarget->tile);
    std::cout << dir.x << "--" << dir.y << std::endl;
    unit->setDirection(dir);



}

