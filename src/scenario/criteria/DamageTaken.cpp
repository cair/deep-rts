//
// Created by per on 9/6/21.
//

#include "scenario/criterias/DamageTaken.h"
#include "Player.h"
DeepRTS::Criteria::DamageTaken::DamageTaken(int damageTakenLimit, int rewardSuccess, int rewardFailure)
        : ScenarioCriteria(rewardSuccess, rewardFailure)
        , damageTakenLimit(damageTakenLimit){

}

bool DeepRTS::Criteria::DamageTaken::evaluate(const Player &player) {
    isValid = player.sDamageTaken >= damageTakenLimit;
    return isValid;
}

int DeepRTS::Criteria::DamageTaken::reward() const {
    return (isValid) ? rewardSuccess : rewardFailure;
}
