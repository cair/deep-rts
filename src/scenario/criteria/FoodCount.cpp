//
// Created by per on 9/6/21.
//

#include "scenario/criterias/FoodCount.h"
#include "Player.h"

DeepRTS::Criteria::FoodCount::FoodCount(int foodCountLimit, int rewardSuccess, int rewardFailure)
        : ScenarioCriteria(rewardSuccess, rewardFailure)
        , foodCountLimit(foodCountLimit){

}

bool DeepRTS::Criteria::FoodCount::evaluate(const Player &player) {
    isValid = player.food >= foodCountLimit;
    return isValid;
}

int DeepRTS::Criteria::FoodCount::reward() const {
    return (isValid) ? rewardSuccess : rewardFailure;
}
