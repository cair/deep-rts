//
// Created by per on 9/6/21.
//

#include "LumberCollect.h"
#include "Player.h"
DeepRTS::Criteria::LumberCollect::LumberCollect(int lumberCollectLimit, int rewardSuccess, int rewardFailure)
        : ScenarioCriteria(rewardSuccess, rewardFailure)
        , lumberCollectLimit(lumberCollectLimit){

}

bool DeepRTS::Criteria::LumberCollect::evaluate(const Player &player) {
    isValid = player.sGatheredLumber >= lumberCollectLimit;
    return isValid;
}

int DeepRTS::Criteria::LumberCollect::reward() const {
    return (isValid) ? rewardSuccess : rewardFailure;
}
