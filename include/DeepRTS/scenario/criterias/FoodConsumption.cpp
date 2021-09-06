//
// Created by per on 9/6/21.
//

#include "FoodConsumption.h"
#include "Player.h"

DeepRTS::Criteria::FoodConsumption::FoodConsumption(int foodConsumptionLimit, int rewardSuccess, int rewardFailure)
        : ScenarioCriteria(rewardSuccess, rewardFailure)
        , foodConsumptionLimit(foodConsumptionLimit){

}

bool DeepRTS::Criteria::FoodConsumption::evaluate(const Player &player) {
    isValid = player.foodConsumption >= foodConsumptionLimit;
    return isValid;
}

int DeepRTS::Criteria::FoodConsumption::reward() const {
    return (isValid) ? rewardSuccess : rewardFailure;
}
