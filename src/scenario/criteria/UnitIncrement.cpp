//
// Created by per on 9/6/21.
//

#include "scenario/criterias/UnitIncrement.h"
#include "Player.h"
DeepRTS::Criteria::UnitIncrement::UnitIncrement(Constants::Unit unitType, int amount, int rewardSuccess, int rewardFailure)
        : ScenarioCriteria(rewardSuccess, rewardFailure)
        , unitType(unitType){

}

bool DeepRTS::Criteria::UnitIncrement::evaluate(const Player &player) {
    bool state = false;
    switch (unitType) {
        case Constants::Peasant:
            state = player.num_peasant > lastValue;
            lastValue = player.num_peasant;
            break;
        case Constants::Peon:
            state = player.num_peasant > lastValue;
            lastValue = player.num_peasant;
            break;
        case Constants::TownHall:
            state = player.num_town_hall > lastValue;
            lastValue = player.num_town_hall;
            break;
        case Constants::Barracks:
            state = player.num_barrack > lastValue;
            lastValue = player.num_barrack;
            break;
        case Constants::Footman:
            state = player.num_footman > lastValue;
            lastValue = player.num_footman;
            break;
        case Constants::Farm:
            state = player.num_farm > lastValue;
            lastValue = player.num_farm;
            break;
        case Constants::Archer:
            state = player.num_archer > lastValue;
            lastValue = player.num_archer;
            break;
        case Constants::None:
            throw std::runtime_error("Incorrect unit type");
    }

    isValid = state;
    return isValid;
}

int DeepRTS::Criteria::UnitIncrement::reward() const {
    return (isValid) ? rewardSuccess : rewardFailure;
}
