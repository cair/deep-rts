//
// Created by Per-Arne on 26.02.2017.
//
#include "Harvesting.h"
#include "../unit/Unit.h"
#include "../util/Pathfinder.h"
#include "../player/Player.h"
#include "../Game.h"

void Harvesting::update(Unit & unit){

    if (unit.harvestIterator == 0) {
        // Go to harvestable_tile;

        Tile *harvestTarget = unit.getTile(unit.harvestTargetID);


        if(unit.distance(*harvestTarget) > 1) {
            // Must walk
            // Find closest walkable tile to the target
            Tile* closestWalkable = Pathfinder::find_first_walkable_tile(harvestTarget);

            // If distance is > 1, change harvest_target
            if(closestWalkable->distance(*harvestTarget) > 1) {
                Tile *closestHarvestable = Pathfinder::find_first_harvestable_tile(closestWalkable);
                if(!closestHarvestable){
                    unit.transitionState(unit.stateManager->idleState);
                    return;
                }
                unit.harvestTargetID = closestHarvestable->id;


            }

            unit.walkingGoalID = closestWalkable->id;
            unit.transitionState(unit.stateManager->walkingState);
            unit.enqueueState(unit.stateManager->harvestingState);
        }

        unit.harvestIterator = 1;




    } else if (unit.harvestIterator == 1) {
        // Start harvesting
        Tile *harvestTarget = unit.getTile(unit.harvestTargetID);


        if(unit.harvestTimer < unit.harvestInterval) {
            unit.harvestTimer += 1;
            return;
        }

        if(unit.lumberCarry + unit.goldCarry + unit.oilCarry >= unit.carryCapacity) {
            // Return to base
            unit.harvestIterator = 2;
            return;
        }

        unit.lumberCarry += harvestTarget->lumberYield;
        unit.goldCarry += harvestTarget->goldYield;
        unit.oilCarry += harvestTarget->oilYield;
        unit.player_.sGatheredGold += harvestTarget->goldYield;
        unit.player_.sGatheredLumber += harvestTarget->lumberYield;
        unit.player_.sGatheredOil += harvestTarget->oilYield;
        harvestTarget->takeResource(harvestTarget->lumberYield + harvestTarget->goldYield + harvestTarget->oilYield);

        // Callback
        unit.game->_onResourceGather(*harvestTarget, unit);

        // No more resources // TODO constant parameter Config
        if(harvestTarget->getResources() <= 0) {

            harvestTarget->setDepleted();
            // Callback
            unit.game->_onResourceDepleted(*harvestTarget, unit);

            // Find new harvestable
            Tile *closestHarvestable = Pathfinder::find_first_harvestable_tile(harvestTarget);
            if(!closestHarvestable){
                unit.transitionState(unit.stateManager->idleState);
                return;
            }


            unit.harvestTargetID = closestHarvestable->id;
        }

        unit.harvestTimer = 0;

    } else if (unit.harvestIterator == 2) {
        // Recall (Walking)
        Unit* closestBase = unit.closestRecallBuilding();

        // No base to recall to
        if (!closestBase) {
            unit.transitionState();
            return;
        }


        if(unit.distance(*closestBase) > 1) {
            // Must walk
            unit.walkingGoalID = closestBase->tile->id;
            unit.transitionState(unit.stateManager->walkingState);
            unit.enqueueState(unit.stateManager->harvestingState);
            return;
        } else {
            // Can deliver
            unit.player_.addGold(unit.goldCarry);
            unit.player_.addLumber(unit.lumberCarry);
            unit.player_.addOil(unit.oilCarry);

            unit.goldCarry = 0;
            unit.lumberCarry = 0;
            unit.oilCarry = 0;

            if(unit.config.harvestForever)
            {
                unit.transitionState(unit.stateManager->harvestingState);
            }else{
                unit.transitionState();
            }
        }

    }
}

void Harvesting::end(Unit & unit){
    (void)(unit);
}

void Harvesting::init(Unit & unit){
    unit.harvestTimer = 0;
    unit.harvestIterator = 0;
    unit.tile->triggerOnTileChange();

}

