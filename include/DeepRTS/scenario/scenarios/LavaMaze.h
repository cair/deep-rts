//
// Created by per on 9/11/21.
//

#ifndef DEEPRTS_LAVAMAZE_H
#define DEEPRTS_LAVAMAZE_H

#include <scenario/Scenario.h>

namespace DeepRTS::Scenarios{

    class LavaMaze: public Scenario{

    public:
        LavaMaze();
        [[maybe_unused]] ActionSequenceContainer optimalStrategy() override;
        Config getConfig();
    };

}

#endif //DEEPRTS_LAVAMAZE_H
