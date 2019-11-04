//
// Created by per on 06.06.18.
//

#include "PyGUI.h"

#include <pybind11/embed.h> // everything needed for embedding
#include "../Game.h"


PyGUI::PyGUI(Game& game): game(game) {
    pybind11::initialize_interpreter();

    this->init_dependencies();
    this->init_argv();
    //this->init_cython();
    this->init_gui();
}


PyGUI::~PyGUI() {
    //pybind11::finalize_interpreter();
}

void PyGUI::init_dependencies(){
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

void PyGUI::init_argv(){
    pybind11::module sys = pybind11::module::import("sys");
    auto args = pybind11::list();
    args.append("");
    sys.attr("argv") = args;
}

void PyGUI::init_cython(){
    pybind11::module pyximport = pybind11::module::import("pyximport");
    pyximport.attr("install")();
}

void PyGUI::init_gui(){
    pybind11::module DeepRTS = pybind11::module::import("DeepRTS");
    auto DeepRTS_Python = DeepRTS.attr("python");
    auto GUI = DeepRTS_Python.attr("GUI");
    auto Game = DeepRTS_Python.attr("Game");
    auto Config = DeepRTS_Python.attr("Config");

    //auto gameInstance = Game("", &this->game);

    /*
     *     gui_config = python.Config(
            render=True,
            view=True,
            inputs=True,
            caption=False,
            unit_health=True,
            unit_outline=False,
            unit_animation=True,
            audio=True,
            audio_volume=50
    )
     */

    //
    //pybind11::setattr(&game, "_on_tile_change", Game.attr("_on_tile_change"))
    gui = GUI(&game, 32, Config(true, true, true, false, true, false, true, false, 50));






    this->gui_attr_render = gui.attr("render");
    this->gui_attr_view = gui.attr("view");

}

void PyGUI::capture() {
    gui.attr("capture")();
}

void PyGUI::view() {
    gui_attr_view();
}

void PyGUI::render() {
    gui_attr_render();
}

