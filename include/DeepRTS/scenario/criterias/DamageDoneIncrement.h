//
// Created by per on 9/6/21.
//

#ifndef DEEPRTS_DAMAGEDONEINCREMENT_H
#define DEEPRTS_DAMAGEDONEINCREMENT_H


#include "ScenarioCriteria.h"
namespace DeepRTS::Criteria{
    class DamageDoneIncrement: public ScenarioCriteria {
        int amount;
        int lastValue = 0;

        bool isValid;
    public:
        explicit DamageDoneIncrement(int amount, int rewardSuccess=0.01, int rewardFailure=-0.01);

        [[nodiscard]] bool evaluate(const Player& player) override;
        [[nodiscard]] int reward() const override;
        [[nodiscard]] std::shared_ptr<ScenarioCriteria> clone() const override{
            return std::shared_ptr<ScenarioCriteria>(new DamageDoneIncrement(*this));
        }
    };

}


#endif //DEEPRTS_DAMAGEDONEINCREMENT_H
