
#include "PyAPI.h"
#include "../../Game.h"
#include "../../player/Player.h"
#include "../../proto/GameMessage.pb.h"
#include <flatbuffers\flatbuffers.h>
#include <thread>
#include <stdlib.h>

bool PyAPI::loaded = false;
int PyAPI::slotCounter = 0;
std::array<PyAPI *, 16> PyAPI::slots;
std::vector<PyAPI *> PyAPI::instances;
std::vector<PyAPI *> PyAPI::freeInstances;


void PyAPI::init() {
	PyImport_AppendInittab("PyAPIRegistry", &PyAPI::PyInit_PyAPIRegistry);
	Py_Initialize();
	slots.fill(NULL);
	std::thread t = std::thread(&PyAPI::start);
	t.detach();

}


void PyAPI::createInstance(std::string scriptName, uint16_t gameID, uint16_t playerID) {
	
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

	pName = PyUnicode_DecodeFSDefault("Main.py");
	std::cout << "Loading python environment. Please wait!" << std::endl;
	PyAPI::loaded = false;
	pModule = PyImport_Import(pName);
	PyAPI::loaded = true;
	std::cout << "Load done! Python started!" << std::endl;
	Py_DECREF(pName);

	if (pModule == NULL) {
		PyErr_Print();
		fprintf(stderr, "Failed to load \"%s\"\n", "Main.py");
		return;
	}
}

PyAPI::PyAPI(uint16_t gameID, uint16_t playerID):
	game(Game::getGame(gameID)), 
	Algorithm(*Game::getGame(gameID)->players[playerID])
{


}



void PyAPI::update()
{
}

void PyAPI::terminal()
{
}

void PyAPI::defineActionSpace()
{
}

std::shared_ptr<BaseAction> PyAPI::findBestAction(std::shared_ptr<Unit> unit)
{
	return std::shared_ptr<BaseAction>();
}

void PyAPI::doAction(std::shared_ptr<BaseAction> action)
{
}

PyObject*  PyAPI::registry_free(PyObject* self, PyObject* args)
{
	// Free a memory address
	PyObject *a;
	if (PyArg_UnpackTuple(args, "", 1, 1, &a))
	{
		size_t ptr = PyLong_AsLongLong(a);
		free((void *)ptr);
		return PyLong_FromLong(1);
	}

	return PyLong_FromLong(-1);
}

PyObject* PyAPI::registry_hook(PyObject* self, PyObject* args)
{
	PyObject *gameID;
	PyObject *playerID;
	if (PyArg_UnpackTuple(args, "", 2, 2, &gameID, &playerID))
	{

		PyAPI *api = new PyAPI(PyLong_AsLongLong(gameID), PyLong_AsLongLong(playerID));
		intptr_t ptr = reinterpret_cast<intptr_t>(api);

		return PyLong_FromLongLong(ptr);
	}
	else {
		PyErr_Print();
		fprintf(stderr, "Failed to start as you are not inserting correct parameters (hook(float, float))  (hook(0, 0))");
		return PyLong_FromLongLong(-1);
	}


	
	return PyLong_FromLongLong(-1);
}

PyObject* PyAPI::registry_get_state(PyObject* self, PyObject* args)
{
	PyObject *aiID;
	PyAPI * api_ptr;
	if (PyArg_UnpackTuple(args, "", 1, 1, &aiID))
	{
		api_ptr = (PyAPI *)PyLong_AsLongLong(aiID);
	}
	else {
		PyErr_Print();
		return PyLong_FromLongLong(-1);
	}

	GameMessage gameMessage = GameMessage();
	api_ptr->game->serialize(gameMessage);


	int size = gameMessage.ByteSize();
	char* array = new char[size];
	gameMessage.SerializeToArray(array, size);


	// Capsulate flatbuffer pointer into an PyObject. this ensure that the pointer type is preserved when retrieving in python
	//PyObject *buffer_capsule_ptr = PyCapsule_New((void *)array, "X_C_API", NULL);
	PyObject *ptr = PyLong_FromLongLong(reinterpret_cast<intptr_t>(array));
	PyObject *bytesize = PyLong_FromLong(size);

	args = PyTuple_New(2);
	PyTuple_SetItem(args, 0, ptr);
	PyTuple_SetItem(args, 1, bytesize);

	return args;
}

static struct PyMethodDef methods[] = {
	{ "hook", PyAPI::registry_hook, METH_VARARGS, "Returns the number" },
	{ "get_state", PyAPI::registry_get_state, METH_VARARGS, "Show a number" },
	{ "free", PyAPI::registry_free, METH_VARARGS, "Show a number" },
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
