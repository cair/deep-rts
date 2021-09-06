//
// Created by per on 9/6/21.
//

#include "StoneCollect.h"
#include "Player.h"

DeepRTS::Criteria::StoneCollect::StoneCollect(int stoneCollectLimit, int rewardSuccess, int rewardFailure)
        : ScenarioCriteria(rewardSuccess, rewardFailure)
        , stoneCollectLimit(stoneCollectLimit){

}

bool DeepRTS::Criteria::StoneCollect::evaluate(const Player &player) {
    isValid = player.sGatheredStone > stoneCollectLimit;
    return isValid;
}

int DeepRTS::Criteria::StoneCollect::reward() const {
    return (isValid) ? rewardSuccess : rewardFailure;
}
