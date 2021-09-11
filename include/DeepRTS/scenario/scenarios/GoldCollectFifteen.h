//
// Created by per on 9/6/21.
//

#ifndef DEEPRTS_GOLDCOLLECTFIFTEEN_H
#define DEEPRTS_GOLDCOLLECTFIFTEEN_H

#include <scenario/Scenario.h>

namespace DeepRTS::Scenarios{

    class GoldCollectFifteen: public Scenario{

    public:
        GoldCollectFifteen();
        [[maybe_unused]] ActionSequenceContainer optimalStrategy() override;



    };



}
#endif //DEEPRTS_GOLDCOLLECTFIFTEEN_H
