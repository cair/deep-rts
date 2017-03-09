
#include "PyAPI.h"
#include "../../Game.h"
#include "../../player/Player.h"
#include "../../proto/GameMessage.pb.h"
#include <thread>

int PyAPI::slotCounter = 0;
std::array<PyAPI *, 16> PyAPI::slots;
std::vector<PyAPI *> PyAPI::instances;
std::vector<PyAPI *> PyAPI::freeInstances;


void PyAPI::init() {
	slots.fill(NULL);
}


void PyAPI::createInstance(std::string scriptName, uint16_t gameID, uint16_t playerID) {
	std::shared_ptr<PyAPI> ai = std::shared_ptr<PyAPI>(new PyAPI(scriptName, gameID, playerID));
	std::thread t = std::thread(&PyAPI::start, ai);
	t.detach();

}

void PyAPI::start() {


	PyObject *pName, *pModule, *pDict, *pFunc;
	PyObject *pArgs, *pValue;
	int i;

	int argc;
	wchar_t * argv[3];
	argc = 3;
	argv[0] = L"Main";
	argv[1] = L"-m";
	argv[2] = L"/tmp/targets.list";


	Py_SetProgramName(argv[0]);
	PySys_SetArgv(argc, argv);

	// Register Python Instance
	PyAPI::instances.push_back(this);


	pName = PyUnicode_DecodeFSDefault(scriptName.c_str());
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule == NULL) {
		PyErr_Print();
		fprintf(stderr, "Failed to load \"%s\"\n", scriptName);
		return;
	}



	/*pFunc = PyObject_GetAttrString(pModule, "bootstrap");
	pArgs = PyTuple_New(argc - 3);
	pValue = PyObject_CallObject(pFunc, pArgs); // std::make_shared<PyAPI>(this)*/










}

PyAPI::PyAPI(std::string scriptName, uint16_t gameID, uint16_t playerID): 
	game(Game::getGame(gameID)), 
	player(game->players[0]),
	scriptName(scriptName)
{


}

void PyAPI::world(PyObject * self, PyObject * args)
{


}




PyObject* PyAPI::registry_hook(PyObject* self, PyObject* args)
{
	// Hook a pyai
	if (PyAPI::instances.empty()) {
		std::cout << "No Free PyAPI instances to hook to!" << std::endl;
		return PyLong_FromLong(-1);
	}

	int id = PyAPI::slotCounter++;
	std::cout << "Register PyAI with id: " << std::to_string(id) << std::endl;
	PyAPI *instance = PyAPI::instances.back();
	PyAPI::instances.pop_back();

	PyAPI::slots[id] = instance;

	//printf_s("C++: show(%ld)\n", PyLong_AsLong(a));


	return PyLong_FromLong(id);
}

PyObject* PyAPI::registry_get_state(PyObject* self, PyObject* args)
{
	PyObject *a;
	if (PyArg_UnpackTuple(args, "", 1, 1, &a))
	{
		long aiID = PyLong_AsLong(a);

		GameMessage state;
		PyAPI::slots[aiID]->game->serialize(state);
		std::string s;

		state.SerializeToString(&s);
		return PyByteArray_FromStringAndSize(s.c_str(), s.size());

	}

	return PyLong_FromLong(-1);
}

static struct PyMethodDef methods[] = {
	{ "hook", PyAPI::registry_hook, METH_VARARGS, "Returns the number" },
	{ "get_state", PyAPI::registry_get_state, METH_VARARGS, "Show a number" },
	{ NULL, NULL, 0, NULL }
};

static struct PyModuleDef modDef = {
	PyModuleDef_HEAD_INIT, "PyAPIRegistry", NULL, -1, methods,
	NULL, NULL, NULL, NULL
};

PyObject* PyAPI::PyInit_PyAPIRegistry(void)
{
	return PyModule_Create(&modDef);
}
