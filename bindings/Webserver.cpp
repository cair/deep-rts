//
// Created by per on 1/7/21.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <string>
#include "../src/contrib/webserver/Webserver.h"



void init_Webserver(py::module &m) {
    py::class_<Webserver>(m, "Webserver")
            .def(py::init<const std::string&, int, bool>())
            .def("start", &Webserver::start);
}