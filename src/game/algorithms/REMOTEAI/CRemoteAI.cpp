#include "CRemoteAI.h"
#include "../../Game.h"
#include "../../proto/GameMessage.pb.h"
#include "Python.h"

pybind11::object import(const std::string& module, const std::string& path, pybind11::object& globals)
{
	pybind11::dict locals;
	locals["module_name"] = pybind11::cast(module); // have to cast the std::string first
	locals["path"] = pybind11::cast(path);


	pybind11::eval<pybind11::eval_statements>(            // tell eval we're passing multiple statements
		"import imp\n"
		"new_module = imp.load_module(module_name, open(path), path, ('py', 'U', imp.PY_SOURCE))\n",
		globals,
		locals);

	return locals["new_module"];
}


PyObject *InitRemoteAI(void) {

	pybind11::module m("CRemoteAI", "my hello world Python module with pybind11");

	pybind11::class_<CRemoteAI>(m, "CRemoteAI")
		.def("getState", &CRemoteAI::getState);

	// Not positive if incrementing is required
	return m.ptr();
}

void CRemoteAI::PySetupImport()
{
	PyImport_AppendInittab("RemoteAI", &InitRemoteAI);
}

void CRemoteAI::PyRunImport()
{
	PyRun_SimpleString("import RemoteAI\nglobals()['CRemoteAI']=RemoteAI.CRemoteAI\nprint(dir(RemoteAI))\nprint(CRemoteAI)");
}

CRemoteAI CRemoteAI::createInstance(int gameID) {
	

	pybind11::object main = pybind11::module::import("__main__");
	pybind11::object globals = main.attr("__dict__");
	pybind11::object module = import("RemoteAI", "RemoteAI.py", globals);
	pybind11::object Strategy = module.attr("RemoteAI");
	pybind11::object strategy = Strategy(gameID);

	return CRemoteAI(strategy, gameID);
}


void CRemoteAI::play()
{
}

std::string CRemoteAI::getState(int gameID)
{

	Game *game = Game::getGame(gameID);
	std::string &msg = game->serialize_json();
	

	return msg;

}

CRemoteAI::CRemoteAI(int gameID): gameID(gameID) {}

CRemoteAI::CRemoteAI(pybind11::object pyInstance, int gameID): pyInstance(pyInstance), gameID(gameID) {
	pyInstance.attr("train")();
}


