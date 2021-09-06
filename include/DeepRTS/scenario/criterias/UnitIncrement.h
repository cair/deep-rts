//
// Created by per on 9/6/21.
//

#ifndef DEEPRTS_UNITINCREMENT_H
#define DEEPRTS_UNITINCREMENT_H
#include "ScenarioCriteria.h"
#include "Constants.h"
namespace DeepRTS::Criteria{
    class UnitIncrement: public ScenarioCriteria {
        int lastValue = 0;
        bool isValid;
        const Constants::Unit unitType;
    public:
        explicit UnitIncrement(Constants::Unit unitType, int amount, int rewardSuccess=0.01, int rewardFailure=-0.01);
        [[nodiscard]] bool evaluate(const Player& player) override;
        [[nodiscard]] int reward() const override;

        [[nodiscard]] std::shared_ptr<ScenarioCriteria> clone() const override{
            return std::shared_ptr<ScenarioCriteria>(new UnitIncrement(*this));
        }

    };

}


#endif //DEEPRTS_UNITINCREMENT_H
