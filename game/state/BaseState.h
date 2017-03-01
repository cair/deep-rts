//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_BASESTATE_H
#define WARC2SIM_BASESTATE_H



#include <string>
#include <memory>
#include "../Constants.h"
class Unit;
class BaseState {


public:
    int id;
    std::string name = "**ERR**";
    BaseState(){};
    virtual void update(std::shared_ptr<Unit> unit)const;
    virtual void init(std::shared_ptr<Unit> unit)const;
    virtual void end(std::shared_ptr<Unit> unit)const;
};


#endif //WARC2SIM_BASESTATE_H
