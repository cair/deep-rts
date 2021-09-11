//
// Created by per on 9/6/21.
//

#ifndef DEEPRTS_GENERALAIONEVERSUSONE_H
#define DEEPRTS_GENERALAIONEVERSUSONE_H
#include "scenario/Scenario.h"

namespace DeepRTS::Scenarios{

    class GeneralAIOneVersusOne: public Scenario{

    public:
        GeneralAIOneVersusOne();


        Config getConfig();
    };



}

#endif //DEEPRTS_GENERALAIONEVERSUSONE_H
