
#include "PyAPI.h"
#include "../../Game.h"
#include "../../player/Player.h"
#include <numpy\arrayobject.h>
#include <thread>
#include <stdlib.h>
#include <mutex>          // std::mutex

int setenv(const char *name, const char *value, int overwrite)
{
	int errcode = 0;
	if (!overwrite) {
		size_t envsize = 0;
		errcode = getenv_s(&envsize, NULL, 0, name);
		if (errcode || envsize) return errcode;
	}
	return _putenv_s(name, value);
}

bool PyAPI::loaded = false;
void PyAPI::init() {
	

	//setenv("PYTHONIOENCODING", "utf-8", 1);

	//PyAPI::start();
	std::thread t = std::thread(&PyAPI::start);
	t.detach();

}


void PyAPI::start() {
	PyImport_AppendInittab("PyAPIRegistry", &PyAPI::PyInit_PyAPIRegistry);
	Py_Initialize();
	PyObject *pName, *pModule;

	int argc;
	wchar_t * argv[3];
	argc = 1;
	argv[0] = L"Main.py";


	Py_SetProgramName(argv[0]);

	PySys_SetArgv(argc, argv);
	pName = PyUnicode_DecodeFSDefault("Main.py");
	std::cout << "Loading python environment. Please wait!" << std::endl;
	PyAPI::loaded = false;
	pModule = PyImport_Import(pName);
	
	Py_DECREF(pName);

	if (pModule == NULL) {
		PyErr_Print();
		fprintf(stderr, "Failed to load \"%s\"\n", "Main.py");
		return;
	}
}

PyAPI::PyAPI(uint8_t gameID, uint8_t playerID):
	game(Game::getGame(gameID)), 
	Algorithm(Game::getGame(gameID)->players[playerID])
{
	// Define and allocate space for state buffer
	G_ROW = 30;
	G_COL = 30;
	G_FEATURES = 28;

	flatStateBufferSize = G_ROW * G_COL * G_FEATURES;
	flatStateBuffer = (int *)malloc(sizeof(int) * flatStateBufferSize);      // allocate MANY ints


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

std::shared_ptr<BaseAction> PyAPI::findBestAction(Unit & unit)
{
	return std::shared_ptr<BaseAction>();
}

void PyAPI::doAction(std::shared_ptr<BaseAction> action)
{

}


PyObject *PyAPI::registry_loaded(PyObject *self, PyObject *args) {
	PyAPI::loaded = true;
	std::cout << "Load done! Python started!" << std::endl;
	return PyLong_FromLong(1);
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


PyObject* PyAPI::registry_do_action(PyObject* self, PyObject* args)
{
	PyObject *aiID;
	PyObject *pyactionID;
	PyAPI * api_ptr;
	int actionID = -1;
	if (PyArg_UnpackTuple(args, "", 2, 2, &aiID, &pyactionID))
	{
		api_ptr = (PyAPI *)PyLong_AsLongLong(aiID);
		actionID = PyLong_AsLong(pyactionID);
	}
	else {
		PyErr_Print();
		return PyLong_FromLongLong(-1);
	}

	if (api_ptr->player.targetedUnit == NULL) {
		api_ptr->player.nextUnit();
	}

	uint16_t prevScore = api_ptr->player.getScore();
	//auto action = api_ptr->getAction(actionID, api_ptr->player.targetedUnit);
	//api_ptr->doAction(action);
	// TODO add TEMPORAL WAIT TIME HERE

	api_ptr->player.queueAction(actionID);

	uint16_t scoreDifference = api_ptr->player.getScore() - prevScore;
	long isTerminal = api_ptr->player.checkDefeat();

	PyObject *reward = PyLong_FromLongLong(scoreDifference);
	PyObject *terminal = PyBool_FromLong(isTerminal);
	PyObject *info = PyLong_FromLongLong(1337);

	args = PyTuple_New(3);
	PyTuple_SetItem(args, 0, reward);
	PyTuple_SetItem(args, 1, terminal);
	PyTuple_SetItem(args, 2, info);
	return args;
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

	int c = 0;
	for (auto &tile : api_ptr->game->map.tiles) {

		/// Tile
		api_ptr->flatStateBuffer[c++] = (int)tile.id;// Layer
		api_ptr->flatStateBuffer[c++] = (int)tile.tileID; // Layer
		api_ptr->flatStateBuffer[c++] = (int)tile.oilYield;// Layer
		api_ptr->flatStateBuffer[c++] = (int)tile.getResources();// Layer
		api_ptr->flatStateBuffer[c++] = (int)tile.lumberYield;// Layer
		api_ptr->flatStateBuffer[c++] = (int)tile.walkable;// Layer
		api_ptr->flatStateBuffer[c++] = (int)tile.harvestable;// Layer
		api_ptr->flatStateBuffer[c++] = (int)tile.swimable;// Layer


		Unit* occupant = tile.getOccupant();
		if (occupant) {
			// Unit
			api_ptr->flatStateBuffer[c++] = (int)occupant->id;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->typeId;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->current_state;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->goldCarry;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->lumberCarry;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->oilCarry;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->carryCapacity;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->direction;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->damageMax;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->damageMin;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->damagePiercing;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->damageRange;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->health;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->health_max;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->military;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->recallable;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->sight;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->structure;// Layer

			// Player
			api_ptr->flatStateBuffer[c++] = (int)occupant->player_->id_;// Layer
			api_ptr->flatStateBuffer[c++] = (int)occupant->player_->faction;// Layer

		}
		else {
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;

			// Player
			api_ptr->flatStateBuffer[c++] = 0;
			api_ptr->flatStateBuffer[c++] = 0;
		}

	}

	PyObject *buf_ptr = PyLong_FromLongLong(reinterpret_cast<intptr_t>(api_ptr->flatStateBuffer));
	PyObject *buf_len = PyLong_FromLongLong(api_ptr->flatStateBufferSize);
	PyObject *buf_rows = PyLong_FromLongLong(api_ptr->G_ROW);
	PyObject *buf_cols = PyLong_FromLongLong(api_ptr->G_COL);
	PyObject *buf_depth = PyLong_FromLongLong(api_ptr->G_FEATURES);

	args = PyTuple_New(5);
	PyTuple_SetItem(args, 0, buf_ptr);
	PyTuple_SetItem(args, 1, buf_len);
	PyTuple_SetItem(args, 2, buf_rows);
	PyTuple_SetItem(args, 3, buf_cols);
	PyTuple_SetItem(args, 4, buf_depth);
	return args;
}




static struct PyMethodDef methods[] = {
	{ "hook", PyAPI::registry_hook, METH_VARARGS, "Returns the number" },
	{ "get_state", PyAPI::registry_get_state, METH_VARARGS, "Show a number" },
	{ "free", PyAPI::registry_free, METH_VARARGS, "Show a number" },
	{ "loaded", PyAPI::registry_loaded, METH_VARARGS, "Initial dependency loading is done" },
	{ "do_action", PyAPI::registry_do_action, METH_VARARGS, "Do a action" },
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
