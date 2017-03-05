//
// Created by Per-Arne on 26.02.2017.
//

#include <iostream>
#include "Harvesting.h"
#include "../unit/unit.h"
#include "../util/Pathfinder.h"
#include "../player/Player.h"

void Harvesting::update(std::shared_ptr<Unit> unit)const{

    if (unit->harvestIterator == 0) {
        // Go to harvestable_tile;

        if(unit->distance(*unit->harvestTarget) > 1) {
            // Must walk
            // Find closest walkable tile to the target
            Tile* closestWalkable = Pathfinder::find_first_walkable_tile(unit->harvestTarget);

            // If distance is > 1, change harvest_target
            if(closestWalkable->distance(unit->harvestTarget) > 1) {
                Tile *closestHarvestable = Pathfinder::find_first_harvestable_tile(closestWalkable);
                if(!closestHarvestable){
                    unit->transitionState(unit->stateManager.idleState);
                    return;
                }
                unit->harvestTarget = closestHarvestable;


            }

            unit->walkingGoal = closestWalkable;
            unit->transitionState(unit->stateManager.walkingState);
            unit->enqueueState(unit->stateManager.harvestingState);
        }

        unit->harvestIterator = 1;




    } else if (unit->harvestIterator == 1) {
        // Start harvesting

        unit->harvestTimer += 1;
        if(unit->harvestTimer >= unit->harvestInterval) {

            if(unit->lumberCarry + unit->goldCarry + unit->oilCarry >= unit->carryCapacity) {
                // Return to base
                unit->harvestIterator = 2;
                return;
            }

            unit->lumberCarry += unit->harvestTarget->lumberYield;
            unit->goldCarry += unit->harvestTarget->goldYield;
            unit->oilCarry += unit->harvestTarget->oilYield;
            unit->player_.statGoldGather += unit->harvestTarget->goldYield;
            unit->player_.statLumberGather += unit->harvestTarget->lumberYield;
            unit->player_.statOilGather += unit->harvestTarget->oilYield;
            unit->harvestTarget->resources -= unit->harvestTarget->lumberYield + unit->harvestTarget->goldYield + unit->harvestTarget->oilYield;

            // No more resources // TODO constant parameter Config
            if(unit->harvestTarget->resources < 1) {
                unit->harvestTarget->setDepleted();

                // Find new harvestable
                Tile *closestHarvestable = Pathfinder::find_first_harvestable_tile(unit->harvestTarget);
                if(!closestHarvestable){
                    unit->transitionState(unit->stateManager.idleState);
                    return;
                }


                unit->harvestTarget = closestHarvestable;
            }

            unit->harvestTimer = 0;



        }


    } else if (unit->harvestIterator == 2) {
        // Recall (Walking)
        std::shared_ptr<Unit> closestBase = unit->closestRecallBuilding();

        // No base to recall to
        if (!closestBase) {
            unit->transitionState();
            return;;
        }


        if(unit->distance(closestBase) > 1) {
            // Must walk
            unit->walkingGoal = closestBase->tile;
            unit->transitionState(unit->stateManager.walkingState);
            unit->enqueueState(unit->stateManager.harvestingState);
            return;
        } else {
            // Can deliver
            unit->player_.addGold(unit->goldCarry);
            unit->player_.addLumber(unit->lumberCarry);
            unit->player_.addOil(unit->oilCarry);

            unit->goldCarry = 0;
            unit->lumberCarry = 0;
            unit->oilCarry = 0;

            if(!Config::getInstance().getMechanicHarvestReplay())
            {
                unit->transitionState(unit->stateManager.harvestingState);
            }else{
                unit->transitionState();
            }
        }

    }
}

void Harvesting::end(std::shared_ptr<Unit> unit)const{

}

void Harvesting::init(std::shared_ptr<Unit> unit)const{
    unit->harvestTimer = 0;
    unit->harvestIterator = 0;
}

