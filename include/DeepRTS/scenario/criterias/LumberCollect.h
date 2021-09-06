//
// Created by per on 9/6/21.
//

#ifndef DEEPRTS_LUMBERCOLLECT_H
#define DEEPRTS_LUMBERCOLLECT_H


#include "ScenarioCriteria.h"
namespace DeepRTS::Criteria{
    class LumberCollect: public ScenarioCriteria {
        int lumberCollectLimit;
        bool isValid;
    public:
        explicit LumberCollect(int lumberCollectLimit, int rewardSuccess=0.01, int rewardFailure=-0.01);

        [[nodiscard]] bool evaluate(const Player& player) override;
        [[nodiscard]] int reward() const override;
        [[nodiscard]] std::shared_ptr<ScenarioCriteria> clone() const override{
            return std::shared_ptr<ScenarioCriteria>(new LumberCollect(*this));
        }
    };
}


#endif //DEEPRTS_LUMBERCOLLECT_H
