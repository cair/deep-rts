//
// Created by Per-Arne on 27.02.2017.
//

#include "Combat.h"
#include "../unit/Unit.h"
#include "../player/Player.h"
#include <algorithm>    // std::max

void Combat::update(Unit & unit){

    unit.combatTimer = std::min(unit.combatInterval, unit.combatTimer + 1.0);
    if(unit.combatTimer >= unit.combatInterval) {
        Unit *combatTarget = unit.getCombatTarget();


		// If combat target are despawn in some way (Building, Dead, Despawned) stop attacking...
		if (!combatTarget || !combatTarget->tile) {

			unit.combatTargetID = -1;
			unit.combatTimer = unit.combatInterval;
			unit.transitionState();
			return;
		}

        int distance = unit.distance(*combatTarget);
        if(distance > unit.damageRange) {
            // Too far away, Walk

            // Only walk distance difference steps before transition back to attack
            int diff = distance - unit.damageRange;
            unit.stepsLeft = diff;

			unit.walkingGoalID = combatTarget->tile->id;
			unit.transitionState(unit.stateManager->walkingState);
            unit.enqueueState(unit.stateManager->combatState);

        } else {
            // Can attack
            int myDamage = unit.getDamage(*combatTarget);
            combatTarget->afflictDamage(myDamage);
            unit.player_.sDamageDone += myDamage;
            combatTarget->player_.sDamageTaken += myDamage;
            unit.combatTimer = 0;


            if(combatTarget->isDead()){
                unit.combatTargetID = Constants::None;
                unit.combatTimer = unit.combatInterval;
                unit.transitionState();
                return;
            }

            if(combatTarget->state->id == Constants::State::Idle) {
                combatTarget->attack(*unit.tile);
            }

        }


    }

}

void Combat::end(Unit & unit){
    (void)(unit);
}

void Combat::init(Unit & unit){
    Position dir = unit.distanceVector(*unit.getCombatTarget()->tile);
    unit.setDirection(dir);



}

