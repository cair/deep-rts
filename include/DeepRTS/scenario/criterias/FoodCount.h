//
// Created by per on 9/6/21.
//

#ifndef DEEPRTS_FOODCOUNT_H
#define DEEPRTS_FOODCOUNT_H


#include "ScenarioCriteria.h"

namespace DeepRTS::Criteria{
    class FoodCount: public ScenarioCriteria {
        int foodCountLimit;
        bool isValid;
    public:
        explicit FoodCount(int foodCountLimit, int rewardSuccess=0.01, int rewardFailure=-0.01);

        [[nodiscard]] bool evaluate(const Player& player) override;
        [[nodiscard]] int reward() const override;

        [[nodiscard]] std::shared_ptr<ScenarioCriteria> clone() const override{
            return std::shared_ptr<ScenarioCriteria>(new FoodCount(*this));
        }
    };

}



#endif //DEEPRTS_FOODCOUNT_H
