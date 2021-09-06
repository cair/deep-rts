//
// Created by per on 9/6/21.
//

#include "GoldCollectFifteen.h"
#include "Constants.h"
#include "scenario/criterias/GoldCollect.h"

DeepRTS::Scenario::GoldCollectFifteen::GoldCollectFifteen()
: Scenario(
        Constants::Map::TEN,
        Config::defaults(),
        {
            std::make_shared<Criteria::GoldCollect>(10)
        }
){
}

DeepRTS::Scenario::Scenario::ActionSequenceContainer DeepRTS::Scenario::GoldCollectFifteen::optimalStrategy() {
    return {
            {Constants::MoveRight, "Peasant0"},
            {Constants::MoveRight, "Peasant0"},
            {Constants::MoveRight, "Peasant0"},
            {Constants::MoveRight, "Peasant0"},
            {Constants::MoveDownRight, "Peasant0"},
            {Constants::MoveRight, "Peasant0"},
    };
}
