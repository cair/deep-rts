#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "../src/state/BaseState.h"
#include "../src/state/Walking.h"
#include "../src/Constants.h"

void init_BaseState(py::module &m) {

    //py::class_<Walking, std::shared_ptr<Walking>, BaseState>(m, "Walking")
    //    .def(py::init<Constants::State>());


    py::class_<BaseState, std::shared_ptr<BaseState>>(m, "BaseState")
            //.def(py::init<Constants::State>())
            .def_readonly("id", &BaseState::id)
            .def_readonly("name", &BaseState::name);
}
