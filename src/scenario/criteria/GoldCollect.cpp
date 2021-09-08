//
// Created by per on 9/6/21.
//

#include "scenario/criterias/GoldCollect.h"
#include "Player.h"


DeepRTS::Criteria::GoldCollect::GoldCollect(int goldCollectedLimit, int rewardSuccess, int rewardFailure)
        : ScenarioCriteria(rewardSuccess, rewardFailure)
        , goldCollectedLimit(goldCollectedLimit){
}

bool DeepRTS::Criteria::GoldCollect::evaluate(const Player &player) {
    isValid = player.sGatheredGold >= goldCollectedLimit;
    return isValid;
}

int DeepRTS::Criteria::GoldCollect::reward() const {
    return (isValid) ? rewardSuccess : rewardFailure;
}
