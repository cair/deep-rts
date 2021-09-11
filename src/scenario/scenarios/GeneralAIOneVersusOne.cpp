//
// Created by per on 9/6/21.
//

#include <scenario/criterias/ResourceIncrement.h>
#include <scenario/criterias/UnitIncrement.h>
#include <scenario/criterias/DamageDoneIncrement.h>
#include "scenario/scenarios/GeneralAIOneVersusOne.h"
#include "Config.h"

using DeepRTS::Config;

DeepRTS::Scenarios::GeneralAIOneVersusOne::GeneralAIOneVersusOne()
        : Scenario(
        Constants::Map::VASTRANA,
        getConfig(),
        {
                std::make_shared<Criteria::ResourceIncrement>(Constants::Resource::Lumber, 10000),
                std::make_shared<Criteria::ResourceIncrement>(Constants::Resource::Gold, 10000),
                std::make_shared<Criteria::ResourceIncrement>(Constants::Resource::Stone, 10000),
                std::make_shared<Criteria::ResourceIncrement>(Constants::Resource::Food, 10000),
                std::make_shared<Criteria::UnitIncrement>(Constants::Unit::TownHall, 10000),
                std::make_shared<Criteria::UnitIncrement>(Constants::Unit::Barracks, 10000),
                std::make_shared<Criteria::UnitIncrement>(Constants::Unit::Farm, 10000),
                std::make_shared<Criteria::UnitIncrement>(Constants::Unit::Peasant, 10000),
                std::make_shared<Criteria::UnitIncrement>(Constants::Unit::Archer, 10000),
                std::make_shared<Criteria::UnitIncrement>(Constants::Unit::Footman, 10000),
                std::make_shared<Criteria::DamageDoneIncrement>(10000)
        }
){
    addPlayer();
    addPlayer();
}

Config DeepRTS::Scenarios::GeneralAIOneVersusOne::getConfig(){
    Config config;
    config.setBarracks(true);
    config.setFootman(true);
    config.setInstantTownHall(true);
    config.setArcher(true);
    config.setStartLumber(1000);
    config.setStartGold(1000);
    config.setStartStone(1000);
    config.setTickModifier(config.tickModifier);
    config.setInstantBuilding(true);
    return config;
}
