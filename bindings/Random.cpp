//
// Created by per on 11/7/19.
//

#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "../src/util/Random.h"

void init_Random(py::module &m) {

    py::class_<Random, std::unique_ptr<Random, py::nodelete>>(m, "Random")
            //.def(py::init([](){ return std::unique_ptr<Random, py::nodelete>(&Random::getInstance()); }));
            .def_static("action", &Random::randAction)
            .def_static("randint", &Random::randInt);



}