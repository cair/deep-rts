//
// Created by per on 9/6/21.
//

#ifndef DEEPRTS_DAMAGETOKEN_H
#define DEEPRTS_DAMAGETOKEN_H


#include "ScenarioCriteria.h"

namespace DeepRTS::Criteria{
    class DamageTaken: public ScenarioCriteria {
        int damageTakenLimit;
        bool isValid;
    public:
        explicit DamageTaken(int damageTakenLimit, int rewardSuccess=0.01, int rewardFailure=-0.01);

        [[nodiscard]] bool evaluate(const Player& player) override;
        [[nodiscard]] int reward() const override;
        [[nodiscard]] std::shared_ptr<ScenarioCriteria> clone() const override{
            return std::shared_ptr<ScenarioCriteria>(new DamageTaken(*this));
        }

    };

}



#endif //DEEPRTS_DAMAGETOKEN_H
