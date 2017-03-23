//
// Created by Per-Arne on 27.02.2017.
//

#include "Combat.h"
#include "../unit/Unit.h"
#include "../player/Player.h"
#include "../util/Pathfinder.h"


void Combat::update(Unit & unit)const{

    unit.combatTimer += 1;
    if(unit.combatTimer >= unit.combatInterval) {

		// If combat target are despawn in some way (Building, Dead, Despawned) stop attacking...
		if (!unit.combatTarget->tile) {
			unit.combatTarget = NULL;
			unit.combatTimer = 1000;
			unit.transitionState();
			return;
		}

        if(unit.distance(*unit.combatTarget->tile) > unit.damageRange) {
            // Too far away, Walk

			unit.walkingGoal = unit.combatTarget->tile;
			unit.transitionState(unit.stateManager->walkingState);
            unit.enqueueState(unit.stateManager->combatState);

        } else {
            // Can attack
            int myDamage = unit.getDamage(*unit.combatTarget);
            unit.combatTarget->afflictDamage(myDamage);
            unit.player_->statUnitDamageDone += myDamage;
            unit.combatTarget->player_->statUnitDamageTaken += myDamage;
            unit.combatTimer = 0;

            if(unit.combatTarget->isDead()){
                unit.combatTarget = NULL;
                unit.combatTimer = 1000;
                unit.transitionState();
                return;
            }

            if(unit.combatTarget->state->id == Constants::State::Idle) {
                unit.combatTarget->attack(*unit.tile);
            }

        }


    }

}

void Combat::end(Unit & unit)const{

}

void Combat::init(Unit & unit)const{
    Position dir = unit.distanceVector(*unit.combatTarget->tile);
    unit.setDirection(dir);



}

