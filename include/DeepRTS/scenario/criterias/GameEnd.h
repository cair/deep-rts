//
// Created by per on 9/9/21.
//

#ifndef DEEPRTS_GAMEEND_H
#define DEEPRTS_GAMEEND_H


#include "ScenarioCriteria.h"
namespace DeepRTS::Criteria{
    class GameEnd: public ScenarioCriteria {

        bool rewardable{};
    public:
        explicit GameEnd(int rewardSuccess=0.01, int rewardFailure=-0.01);

        [[nodiscard]] bool evaluate(const Player& player) override;
        [[nodiscard]] int reward() const override;
        [[nodiscard]] std::shared_ptr<ScenarioCriteria> clone() const override{
            return std::shared_ptr<ScenarioCriteria>(new GameEnd(*this));
        }

    };

}



#endif //DEEPRTS_GAMEEND_H
