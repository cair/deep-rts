//
// Created by per-arne on 03.04.17.
//

#ifndef DEEPRTS_LOGGAME_H
#define DEEPRTS_LOGGAME_H


#include "LogPlayer.h"

class Game;
class LogGame {

    std::vector<LogPlayer> players;
    rapidjson::Document doc;
    Game &game;
    int ticks;
public:
    LogGame(Game &game);

    void serialize();
    void reset();
    void record();



};



#endif //DEEPRTS_LOGGAME_H
