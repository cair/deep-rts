//
// Created by per on 9/6/21.
//

#ifndef DEEPRTS_SCENARIO_H
#define DEEPRTS_SCENARIO_H

#include <Game.h>
#include "criterias/ScenarioCriteria.h"
namespace DeepRTS{
    class Player;
}

namespace DeepRTS::Scenarios {
    class Scenario: public Game {
    public:

        Criteria::ScenarioCriteria::ScenarioContainer criteriaListTemplate;
        using ActionSequenceContainer = std::vector<std::tuple<Constants::Action, const std::string>>;

        std::array<Criteria::ScenarioCriteria::ScenarioContainer, 8> criteriaList;
        explicit Scenario(
                const std::string& map,
                Config config,
                Criteria::ScenarioCriteria::ScenarioContainer &&criterias
        );

        void update() override;

        [[nodiscard]] bool evaluate(const Player& player);
        [[nodiscard]] int reward(const Player &player);
        [[maybe_unused]] virtual ActionSequenceContainer optimalStrategy();
        std::tuple<int, int, bool> computeOptimalStrategy(Player& player);
        std::tuple<int, int, bool> optimalPlayGameStep(Player &player);

        void reset() override;

        void createCriterionsForPlayers();

    };
}

#endif //DEEPRTS_SCENARIO_H
