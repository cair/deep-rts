//
// Created by per on 9/6/21.
//

#ifndef DEEPRTS_STONECOLLECT_H
#define DEEPRTS_STONECOLLECT_H


#include "ScenarioCriteria.h"
namespace DeepRTS::Criteria{
    class StoneCollect: public ScenarioCriteria {
        int stoneCollectLimit;
        bool isValid;
    public:
        explicit StoneCollect(int stonerCollectLimit, int rewardSuccess=0.01, int rewardFailure=-0.01);

        [[nodiscard]] bool evaluate(const Player& player) override;
        [[nodiscard]] int reward() const override;
        [[nodiscard]] std::shared_ptr<ScenarioCriteria> clone() const override{
            return std::shared_ptr<ScenarioCriteria>(new StoneCollect(*this));
        }

    };
}
#endif //DEEPRTS_STONECOLLECT_H
