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
    Constants::State id = Constants::State::None;
    std::string name = "**ERR**";
    BaseState(Constants::State id): id(id){
    };
    virtual void update(std::shared_ptr<Unit> unit)const;
    virtual void init(std::shared_ptr<Unit> unit)const;
    virtual void end(std::shared_ptr<Unit> unit)const;
};


