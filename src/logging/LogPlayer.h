//
// Created by per-arne on 03.04.17.
//

#ifndef DEEPRTS_LOGPLAYER_H
#define DEEPRTS_LOGPLAYER_H
#include <string>
#include <vector>
#include "LogTick.h"
#include "../Config.h"

class LogPlayer {

public:
    std::vector<LogTick> ticks;
    std::string name;

    LogPlayer() {
        ticks.reserve(Config::getInstance().getTickReset());

    }
};


#endif //DEEPRTS_LOGPLAYER_H
