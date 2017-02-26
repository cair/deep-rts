//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_BASESTATE_H
#define WARC2SIM_BASESTATE_H



#include <string>
#include "../Constants.h"
class Unit;
class BaseState {


public:
    int id;
    std::string name = "**ERR**";
    BaseState(){};
    virtual void update(Unit &player)const;
    virtual void init(Unit &player)const;
    virtual void end(Unit &player)const;
};


#endif //WARC2SIM_BASESTATE_H
