//
// Created by per on 9/6/21.
//

#include "DamageDoneIncrement.h"
#include "Player.h"
DeepRTS::Criteria::DamageDoneIncrement::DamageDoneIncrement(int amount, int rewardSuccess, int rewardFailure)
        : ScenarioCriteria(rewardSuccess, rewardFailure)
        , amount(amount){

}

bool DeepRTS::Criteria::DamageDoneIncrement::evaluate(const Player &player) {
    isValid = player.sDamageDone > lastValue;
    lastValue = player.sDamageDone;
    return player.sDamageDone >= amount;
}

int DeepRTS::Criteria::DamageDoneIncrement::reward() const {
    return (isValid) ? rewardSuccess : rewardFailure;
}
