//
// Created by Per-Arne on 24.02.2017.
//

#pragma once



#include <string>
#include <iostream>
#include <memory>
#include "../Constants.h"
class Unit;
class BaseState {


public:
    Constants::State id = Constants::State::Base;
    std::string name = "**ERR**";
    BaseState(Constants::State id): id(id){
    };
    virtual void update(Unit & unit);
    virtual void init(Unit & unit);
    virtual void end(Unit & unit);
};


