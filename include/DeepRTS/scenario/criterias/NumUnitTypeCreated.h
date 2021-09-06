//
// Created by per on 9/6/21.
//

#ifndef DEEPRTS_NUMUNITTYPECREATED_H
#define DEEPRTS_NUMUNITTYPECREATED_H

#include "Constants.h"
#include "ScenarioCriteria.h"

namespace DeepRTS::Criteria{
    class NumUnitTypeCreated: public ScenarioCriteria {
        const Constants::Unit unitType;
        bool isValid;
        const int count;
    public:
        explicit NumUnitTypeCreated(Constants::Unit unitType, int count, int rewardSuccess=0.01, int rewardFailure=-0.01);

        [[nodiscard]] bool evaluate(const Player& player) override;
        [[nodiscard]] int reward() const override;
        [[nodiscard]] std::shared_ptr<ScenarioCriteria> clone() const override{
            return std::shared_ptr<ScenarioCriteria>(new NumUnitTypeCreated(*this));
        }

    };

}



#endif //DEEPRTS_NUMUNITTYPECREATED_H
