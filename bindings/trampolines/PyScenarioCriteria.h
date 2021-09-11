//
// Created by per on 9/8/21.
//

#ifndef DEEPRTS_PYSCENARIOCRITERIA_H
#define DEEPRTS_PYSCENARIOCRITERIA_H
#include <pybind11/pybind11.h>
#include "scenario/criterias/ScenarioCriteria.h"
using DeepRTS::Criteria::ScenarioCriteria;
using DeepRTS::Game;
using DeepRTS::Player;

class PyScenarioCriteria: public ScenarioCriteria {
    using ScenarioCriteria::ScenarioCriteria;



    [[nodiscard]] std::shared_ptr<ScenarioCriteria> clone() const override {
        PYBIND11_OVERRIDE_PURE(std::shared_ptr<ScenarioCriteria>, PyScenarioCriteria, clone, );
    }

    [[nodiscard]] bool evaluate(const Player& player) override {
        PYBIND11_OVERRIDE_PURE(bool, PyScenarioCriteria, evaluate, );
    }

    [[nodiscard]] int reward() const override {
        PYBIND11_OVERRIDE_PURE(int, PyScenarioCriteria, reward,);
    }
};




#endif //DEEPRTS_PYSCENARIOCRITERIA_H
