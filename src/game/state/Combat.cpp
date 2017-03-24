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
        Unit &combatTarget = unit.getCombatTarget();


		// If combat target are despawn in some way (Building, Dead, Despawned) stop attacking...
		if (!combatTarget.tile) {
			unit.combatTargetID = -1;
			unit.combatTimer = 1000;
			unit.transitionState();
			return;
		}

        if(unit.distance(*combatTarget.tile) > unit.damageRange) {
            // Too far away, Walk

			unit.walkingGoalID = combatTarget.tile->id;
			unit.transitionState(unit.stateManager->walkingState);
            unit.enqueueState(unit.stateManager->combatState);

        } else {
            // Can attack
            int myDamage = unit.getDamage(combatTarget);
            combatTarget.afflictDamage(myDamage);
            unit.player_->statUnitDamageDone += myDamage;
            combatTarget.player_->statUnitDamageTaken += myDamage;
            unit.combatTimer = 0;

            if(combatTarget.isDead()){
                unit.combatTargetID = Constants::None;
                unit.combatTimer = 1000;
                unit.transitionState();
                return;
            }

            if(combatTarget.state->id == Constants::State::Idle) {
                combatTarget.attack(*unit.tile);
            }

        }


    }

}

void Combat::end(Unit & unit)const{

}

void Combat::init(Unit & unit)const{
    Position dir = unit.distanceVector(*unit.getCombatTarget().tile);
    unit.setDirection(dir);



}

