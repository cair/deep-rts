//
// Created by per on 9/6/21.
//

#include "DamageTakenIncrement.h"
#include "Player.h"

DeepRTS::Criteria::DamageTakenIncrement::DamageTakenIncrement(int amount, int rewardSuccess, int rewardFailure)
        : ScenarioCriteria(rewardSuccess, rewardFailure)
        , amount(amount){

}

bool DeepRTS::Criteria::DamageTakenIncrement::evaluate(const Player &player) {
    isValid = player.sDamageTaken > lastValue;
    lastValue = player.sDamageTaken;
    return player.sDamageTaken >= amount;
}

int DeepRTS::Criteria::DamageTakenIncrement::reward() const {
    return (isValid) ? rewardSuccess : rewardFailure;
}
