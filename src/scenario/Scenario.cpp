//
// Created by per on 9/6/21.
//

#include "scenario/Scenario.h"

#include <utility>
#include "Player.h"
#include "unit/Unit.h"
#include "Game.h"

DeepRTS::Scenario::Scenario::Scenario(
        const std::string& map,
        Config config,
        Criteria::ScenarioCriteria::ScenarioContainer &&criteriaList)
: Game(map, std::move(config))
, criteriaListTemplate(criteriaList)
{
    createCriterionsForPlayers();
}

void DeepRTS::Scenario::Scenario::createCriterionsForPlayers(){
    for(int i =0; i < 8; i++){
        Criteria::ScenarioCriteria::ScenarioContainer copied;
        std::transform(criteriaListTemplate.begin(), criteriaListTemplate.end(), std::back_inserter(copied), [](
                std::shared_ptr<Criteria::ScenarioCriteria>& item){
            return item->clone();
        });
        criteriaList[i] = copied;
    }
}

bool DeepRTS::Scenario::Scenario::evaluate(const Player &player){
    auto& criterias = criteriaList.at(player.getId());

    return std::all_of(criterias.begin(), criterias.end(), [&player](auto& criteria){
        return criteria->evaluate(player);
    });
}


int DeepRTS::Scenario::Scenario::reward(const Player &player) {
    auto& criterias = criteriaList.at(player.getId());
    return std::all_of(criterias.begin(), criterias.end(), [](auto& criteria){
        return criteria->reward();
    });
}


[[maybe_unused]] DeepRTS::Scenario::Scenario::ActionSequenceContainer DeepRTS::Scenario::Scenario::optimalStrategy() {
    return {
    };
}

std::tuple<int, int, bool> DeepRTS::Scenario::Scenario::computeOptimalStrategy(Player& player) {
    auto optimalActionSet = optimalStrategy();

    // Set selected player
    setSelectedPlayer(player);

    int totalSteps = 0;
    int totalReward = 0;

    bool initialBuildHandled = false;
    for(auto& [action, unitName]: optimalActionSet){
        auto* unit = getUnitByNameID(unitName);

        if(unit == nullptr){
            throw std::runtime_error("Error in optimal_play_sequence. The unit with ID=" + unitName + "was not found.");
        }

        if(!initialBuildHandled){
            while(unit->state->id == Constants::State::Building){
                optimalPlayGameStep(player);
            }
            initialBuildHandled = true;
        }

        player.setTargetedUnitID(unit->id);

        while(unit->state->id != Constants::State::Idle){
            auto [s, r, t]  = optimalPlayGameStep(player);
            totalSteps += s;
            totalReward += r;
            terminal = t;
        }

        player.do_action(action);

    }

    terminal = false;
    while(!terminal){
        auto [s, r, t] = optimalPlayGameStep(player);
        totalSteps += s;
        totalReward += r;
        terminal = t;
    }


    reset();

    return {totalSteps, totalReward, terminal};

}

std::tuple<int, int, bool> DeepRTS::Scenario::Scenario::optimalPlayGameStep(Player &player) {
    update();
    bool success = evaluate(player);
    int accumulatedReward = reward(player);
    int steps = 1;
    return {steps, accumulatedReward, success};
}

void DeepRTS::Scenario::Scenario::update() {
    Game::update();
    for(auto& player: players){
        auto terminal = evaluate(player);

        if(terminal){
            this->terminal = terminal;
        }

    }
}

void DeepRTS::Scenario::Scenario::reset(){
    Game::reset();
    createCriterionsForPlayers(); // Create new criterions (dont care to reset em manually)
}