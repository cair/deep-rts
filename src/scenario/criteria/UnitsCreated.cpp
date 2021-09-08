//
// Created by per on 9/6/21.
//

#include "scenario/criterias/UnitsCreated.h"
#include "Player.h"
DeepRTS::Criteria::UnitsCreated::UnitsCreated(int unitsCreatedLimit, int rewardSuccess, int rewardFailure)
        : ScenarioCriteria(rewardSuccess, rewardFailure)
        , unitsCreatedLimit(unitsCreatedLimit){

}

bool DeepRTS::Criteria::UnitsCreated::evaluate(const Player &player) {
    isValid = player.sUnitsCreated >= unitsCreatedLimit;
    return isValid;
}

int DeepRTS::Criteria::UnitsCreated::reward() const {
    return (isValid) ? rewardSuccess : rewardFailure;
}
