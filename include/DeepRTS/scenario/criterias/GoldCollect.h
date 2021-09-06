//
// Created by per on 9/6/21.
//

#ifndef DEEPRTS_GOLDCOLLECT_H
#define DEEPRTS_GOLDCOLLECT_H
#include "ScenarioCriteria.h"
namespace DeepRTS::Criteria{
    class GoldCollect: public ScenarioCriteria {
        int goldCollectedLimit;
        bool isValid;
    public:
        explicit GoldCollect(int goldCollectedLimit, int rewardSuccess=0.01, int rewardFailure=-0.01);

        [[nodiscard]] bool evaluate(const Player& player) override;
        [[nodiscard]] int reward() const override;
        [[nodiscard]] std::shared_ptr<ScenarioCriteria> clone() const override{
            return std::shared_ptr<ScenarioCriteria>(new GoldCollect(*this));
        }

    };

}


#endif //DEEPRTS_GOLDCOLLECT_H
