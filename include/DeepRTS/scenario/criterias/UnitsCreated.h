//
// Created by per on 9/6/21.
//

#ifndef DEEPRTS_UNITSCREATED_H
#define DEEPRTS_UNITSCREATED_H


#include "ScenarioCriteria.h"

namespace DeepRTS::Criteria{
    class UnitsCreated: public ScenarioCriteria {
        int unitsCreatedLimit;
        bool isValid;
    public:
        explicit UnitsCreated(int unitsCreatedLimit, int rewardSuccess=0.01, int rewardFailure=-0.01);

        [[nodiscard]] bool evaluate(const Player& player) override;
        [[nodiscard]] int reward() const override;
        [[nodiscard]] std::shared_ptr<ScenarioCriteria> clone() const override{
            return std::shared_ptr<ScenarioCriteria>(new UnitsCreated(*this));
        }

    };

}



#endif //DEEPRTS_UNITSCREATED_H
