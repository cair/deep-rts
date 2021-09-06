//
// Created by per on 9/6/21.
//

#include "ResourceIncrement.h"
#include "Player.h"

DeepRTS::Criteria::ResourceIncrement::ResourceIncrement(Constants::Resource resourceType, int amount, int rewardSuccess, int rewardFailure)
        : ScenarioCriteria(rewardSuccess, rewardFailure)
        , resourceType(resourceType){

}

bool DeepRTS::Criteria::ResourceIncrement::evaluate(const Player &player) {
    bool state;
    switch (resourceType) {
        case Constants::Stone:
            state = player.sGatheredStone > lastValue;
            lastValue = player.sGatheredStone;
            break;
        case Constants::Gold:
            state = player.sGatheredGold > lastValue;
            lastValue = player.sGatheredGold;
            break;
        case Constants::Lumber:
            state = player.sGatheredLumber > lastValue;
            lastValue = player.sGatheredLumber;
            break;
    }
    isValid = state;
    return isValid;
}

int DeepRTS::Criteria::ResourceIncrement::reward() const {
    return (isValid) ? rewardSuccess : rewardFailure;
}
