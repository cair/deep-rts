//
// Created by per on 06.06.18.
//

#include "gui/PyGUI.h"
#include "Tile.h"
#include "Game.h"
#include <pybind11/embed.h> // everything needed for embedding

PyGUI::PyGUI(Game& game): BaseGUI(game) {
    pybind11::initialize_interpreter(); // start the interpreter and keep it alive

    PyGUI::initDependencies();
    PyGUI::initArgv();
    PyGUI::initCython();

    this->initGUI();
}


PyGUI::~PyGUI() {
    pybind11::finalize_interpreter();
}

void PyGUI::initDependencies(){
    pybind11::module subprocess = pybind11::module::import("subprocess");
    pybind11::module sys = pybind11::module::import("sys");
    auto args = pybind11::list();
    args.append(sys.attr("executable"));
    args.append("-m");
    args.append("pip");
    args.append("install");
    args.append("-r");
    args.append("../requirements.txt");
    args.append("--user");
    subprocess.attr("check_call")(args);
}

void PyGUI::initArgv(){
    pybind11::module sys = pybind11::module::import("sys");
    auto args = pybind11::list();
    args.append("");
    sys.attr("argv") = args;
}

void PyGUI::initCython(){
    pybind11::module pyximport = pybind11::module::import("pyximport");
    pyximport.attr("install")();
}

void PyGUI::initGUI(){
    pybind11::module Engine = pybind11::module::import("Engine");
    pybind11::module DeepRTS = pybind11::module::import("DeepRTS");
    pybind11::module::import("pygame");
    auto DeepRTS_Python = DeepRTS.attr("python");
    auto GUI = DeepRTS_Python.attr("GUI");

    auto Config = DeepRTS_Python.attr("Config");

    gui = GUI(&game, 32, Config(true, true, true, false, true, false, true, false, 50));
    gui_attr_on_tile_change = gui.attr("on_tile_change");
    gui_attr_view = gui.attr("view");


}

void PyGUI::onTileChange(const Tile& tile){
    //std::cout << "YES" << std::endl;
    gui_attr_on_tile_change(tile);

}

const cv::Mat& PyGUI::render()const {
    gui.attr("event")();
    gui_attr_view();
    return renderData;
}
