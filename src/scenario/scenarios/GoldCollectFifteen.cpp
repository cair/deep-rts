//
// Created by per on 9/6/21.
//

#include "scenario/scenarios/GoldCollectFifteen.h"
#include "Constants.h"
#include "scenario/criterias/GoldCollect.h"

DeepRTS::Scenarios::GoldCollectFifteen::GoldCollectFifteen()
: Scenario(
        Constants::Map::EBLIL,
        Config::defaults(),
        {
            std::make_shared<Criteria::GoldCollect>(10)
        }
){
    addPlayer();
}

DeepRTS::Scenarios::Scenario::ActionSequenceContainer DeepRTS::Scenarios::GoldCollectFifteen::optimalStrategy() {
    return {
            {Constants::MoveRight, "Peasant0"},
            {Constants::MoveRight, "Peasant0"},
            {Constants::MoveRight, "Peasant0"},
            {Constants::MoveRight, "Peasant0"},
            {Constants::MoveDownRight, "Peasant0"},
            {Constants::MoveRight, "Peasant0"},
    };
}
