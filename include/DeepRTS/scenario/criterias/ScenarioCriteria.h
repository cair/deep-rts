//
// Created by per on 9/6/21.
//

#ifndef DEEPRTS_SCENARIOCRITERIA_H
#define DEEPRTS_SCENARIOCRITERIA_H
#include <vector>
#include "ScenarioCriteria.h"
#include <memory>

namespace DeepRTS{
    class Player;
}




namespace DeepRTS::Criteria {
    class ScenarioCriteria {

    protected:
        const int rewardSuccess;
        const int rewardFailure;
    public:
        using ScenarioContainer = std::vector<std::shared_ptr<ScenarioCriteria>>;
        ScenarioCriteria(int rewardSuccess=0.01, int rewardFailure=-0.01);

        virtual std::shared_ptr<ScenarioCriteria> clone() const = 0;
        virtual bool evaluate(const Player& player) = 0;
        virtual int reward() const = 0;



    };

}

#endif //DEEPRTS_SCENARIOCRITERIA_H
