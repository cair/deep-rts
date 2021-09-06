//
// Created by per on 9/6/21.
//

#ifndef DEEPRTS_FOODCONSUMPTION_H
#define DEEPRTS_FOODCONSUMPTION_H


#include "ScenarioCriteria.h"
namespace DeepRTS::Criteria{
    class FoodConsumption: public ScenarioCriteria {
        int foodConsumptionLimit;
        bool isValid;
    public:
        explicit FoodConsumption(int foodConsumptionLimit, int rewardSuccess=0.01, int rewardFailure=-0.01);

        [[nodiscard]] bool evaluate(const Player& player) override;
        [[nodiscard]] int reward() const override;
        [[nodiscard]] std::shared_ptr<ScenarioCriteria> clone() const override{
            return std::shared_ptr<ScenarioCriteria>(new FoodConsumption(*this));
        }

    };

}



#endif //DEEPRTS_FOODCONSUMPTION_H
