//
// Created by Per-Arne on 24.02.2017.
//

#include "BaseState.h"

void BaseState::update(Unit& unit){
    (void)(unit);
}

void BaseState::init(Unit& unit) {
    (void)(unit);
}

void BaseState::end(Unit& unit){
    (void)(unit);
}

BaseState::~BaseState() = default;