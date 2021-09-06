//
// Created by per on 9/6/21.
//

#ifndef DEEPRTS_DAMAGETAKENINCREMENT_H
#define DEEPRTS_DAMAGETAKENINCREMENT_H

#include "ScenarioCriteria.h"
namespace DeepRTS::Criteria{
    class DamageTakenIncrement: public ScenarioCriteria {
        int amount;
        int lastValue = 0;
        bool isValid;
    public:
        explicit DamageTakenIncrement(int amount, int rewardSuccess=0.01, int rewardFailure=-0.01);

        [[nodiscard]] bool evaluate(const Player& player) override;

        [[nodiscard]] int reward() const override;
        [[nodiscard]] std::shared_ptr<ScenarioCriteria> clone() const override{
            return std::shared_ptr<ScenarioCriteria>(new DamageTakenIncrement(*this));
        }

    };

}



#endif //DEEPRTS_DAMAGETAKENINCREMENT_H
