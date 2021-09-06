//
// Created by per on 9/6/21.
//

#ifndef DEEPRTS_RESOURCEINCREMENT_H
#define DEEPRTS_RESOURCEINCREMENT_H

#include "ScenarioCriteria.h"
#include "Constants.h"
namespace DeepRTS::Criteria{
    class ResourceIncrement: public ScenarioCriteria {
        int lastValue = 0;
        bool isValid;
        const Constants::Resource resourceType;
    public:
        explicit ResourceIncrement(Constants::Resource resourceType, int amount, int rewardSuccess=0.01, int rewardFailure=-0.01);
        [[nodiscard]] bool evaluate(const Player& player) override;
        [[nodiscard]] int reward() const override;
        [[nodiscard]] std::shared_ptr<ScenarioCriteria> clone() const override{
            return std::shared_ptr<ScenarioCriteria>(new ResourceIncrement(*this));
        }


    };

}



#endif //DEEPRTS_RESOURCEINCREMENT_H
