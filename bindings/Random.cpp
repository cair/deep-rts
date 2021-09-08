//
// Created by per on 11/7/19.
//

#include <pybind11/pybind11.h>
#include <effolkronium/random.hpp>
namespace py = pybind11;


void init_Random(py::module &m) {
    using Random = effolkronium::random_static;
    Random::get<int>(0, 1);
    py::class_<Random, std::unique_ptr<Random, py::nodelete>>(m, "Random")
            //.def(py::init([](){ return std::unique_ptr<Random, py::nodelete>(&Random::getInstance()); }));
            .def_static("get", [](int a, int b){
                return Random::get(a, b);
            });



}