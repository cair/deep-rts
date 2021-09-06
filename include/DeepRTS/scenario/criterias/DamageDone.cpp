//
// Created by per on 9/6/21.
//

#include "DamageDone.h"
#include "Player.h"
DeepRTS::Criteria::DamageDone::DamageDone(int damageDone, int rewardSuccess, int rewardFailure)
: ScenarioCriteria(rewardSuccess, rewardFailure)
, damageDone(damageDone){

}

bool DeepRTS::Criteria::DamageDone::evaluate(const Player &player) {
    isValid = player.sDamageDone >= damageDone;
    return isValid;
}

int DeepRTS::Criteria::DamageDone::reward() const {
    return (isValid) ? rewardSuccess : rewardFailure;
}
