//
// Created by per on 9/6/21.
//

#ifndef DEEPRTS_DAMAGEDONE_H
#define DEEPRTS_DAMAGEDONE_H


#include "ScenarioCriteria.h"
namespace DeepRTS::Criteria{
    class DamageDone: public ScenarioCriteria {
        int damageDone;
        bool isValid;
    public:
        explicit DamageDone(int damageDone, int rewardSuccess=0.01, int rewardFailure=-0.01);

        [[nodiscard]] bool evaluate(const Player& player) override;

        [[nodiscard]] int reward() const override;

        [[nodiscard]] std::shared_ptr<ScenarioCriteria> clone() const override{
            return std::shared_ptr<ScenarioCriteria>(new DamageDone(*this));
        }

    };

}




#endif //DEEPRTS_DAMAGEDONE_H
