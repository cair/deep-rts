//
// Created by per on 9/6/21.
//

#include "NumUnitTypeCreated.h"
#include "Player.h"

bool DeepRTS::Criteria::NumUnitTypeCreated::evaluate(const Player &player) {
    switch (unitType) {
        case Constants::Peasant:
            isValid = player.num_peasant >= count;
            break;
        case Constants::Peon:
            isValid = player.num_peasant >= count; // TODO
            break;
        case Constants::TownHall:
            isValid = player.num_town_hall >= count;
            break;
        case Constants::Barracks:
            isValid = player.num_barrack >= count;
            break;
        case Constants::Footman:
            isValid = player.num_footman >= count;
            break;
        case Constants::Farm:
            isValid = player.num_farm >= count;
            break;
        case Constants::Archer:
            isValid = player.num_archer >= count;
            break;
        case Constants::None:
            throw std::runtime_error("Unhandled unit type in NumUnitTypeCreated criteria");
    }
    return isValid;
}

DeepRTS::Criteria::NumUnitTypeCreated::NumUnitTypeCreated(Constants::Unit unitType, int count, int rewardSuccess, int rewardFailure)
: ScenarioCriteria(rewardSuccess, rewardFailure)
, unitType(unitType)
, count(count){}

int DeepRTS::Criteria::NumUnitTypeCreated::reward() const {
    return (isValid) ? rewardSuccess : rewardFailure;
}
