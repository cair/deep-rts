//
// Created by per on 9/11/21.
//
#include <scenario/criterias/ResourceIncrement.h>
#include <scenario/scenarios/GeneralAIOneVersusOne.h>
#include "scenario/scenarios/LavaMaze.h"


DeepRTS::Scenarios::LavaMaze::LavaMaze()
        : Scenario(
        Constants::Map::GONDOR,
        getConfig(),
        {
            std::make_shared<Criteria::ResourceIncrement>(Constants::Resource::Stone, 1)
        })
{
    addPlayer();
}

DeepRTS::Scenarios::Scenario::ActionSequenceContainer DeepRTS::Scenarios::LavaMaze::optimalStrategy() {
    return Scenario::optimalStrategy();
}

DeepRTS::Config DeepRTS::Scenarios::LavaMaze::getConfig(){
    Config config;
    config.setBarracks(true);
    config.setFootman(true);
    config.setInstantTownHall(true);
    config.setArcher(true);
    config.setStartLumber(0);
    config.setStartGold(0);
    config.setStartStone(0);
    config.setTileDamageModifier(100.0);
    config.setTickModifier(config.tickModifier);
    config.setInstantBuilding(true);
    return config;
}

