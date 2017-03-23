
#include "PyAI.h"
#include "../../Game.h"
#include "../../player/Player.h"
#include <thread>
#include <mutex>          // std::mutex
#include "../base/AIRepository.h"

void PyAI::start() {
	PyImport_AppendInittab("PyAPIRegistry", &PyAI::PyInit_PyAPIRegistry);
	Py_Initialize();
	PyObject *pName, *pModule;

	int argc = 1; // Argument count
	wchar_t * argv[3];
	argv[0] = L"Main.py";


	Py_SetProgramName(argv[0]);	// Set application name
	PySys_SetArgv(argc, argv); // Set arguments

	pName = PyUnicode_FromString("Main");
	//pName = PyUnicode_DecodeFSDefault("Main");	// Name of module
	std::cout << "Loading python environment. Please wait!" << std::endl;
	pModule = PyImport_Import(pName); // Get module into C++
	Py_DECREF(pName);


	if (pModule == NULL) {
		PyErr_Print();
		fprintf(stderr, "Failed to load \"%s\"\n", "Main.py");
		return;
	}

	Py_DECREF(pModule);


	
}

PyAI::PyAI(PyObject *pyaiInstance) :
	pyaiInstance(pyaiInstance),
	game(Game::getGame(0)),
	Algorithm(NULL)
{
	///
	/// Set name of the PyAI
	///
	PyObject *aiName = PyObject_GetAttrString(pyaiInstance, "name");
	assert(aiName && "Name argument is missing in your PyAI class");
	std::string name = std::string(PyUnicode_AsUTF8(aiName));
	assert(name.length() > 0 && "Name is not set!");
	setName(name);

	///
	/// Define and allocate space for state buffer
	///
	G_ROW = 30;
	G_COL = 30;
	G_FEATURES = 8;

	stateBufferSize = G_ROW * G_COL * G_FEATURES;
	stateBuffer = (char *)malloc(sizeof(char) * stateBufferSize);      // allocate MANY ints
	for (int i = 0; i < stateBufferSize; i++) {
		stateBuffer[i] = 0;
	}

	///
	/// Update State data for AI
	///
	updateState();
}
	
	

void PyAI::update()
{
	///
	/// Do nothing when defeated
	///
	if (player->defeated) {
		return;
	}

	updateState(); // Update state on python side (Python)


	int actionID = findAction(); // Determine which action to take (Python)

	train(); // Train AI (Python)

	auto actionC = getAction(actionID); // Translate Action (C++)

	player->queueAction(actionC); // Queue action (C++)


}

void PyAI::reset()
{
	lastPlayerScore = player->getScore();
	PyObject *pyReset = PyObject_CallMethod(pyaiInstance, "reset", NULL); // PyObject_CallMethodObjArgs faster?
	if (pyReset == NULL) {
		PyErr_Print();
		return;
	}
	Py_DECREF(pyReset);
	return;
}

void PyAI::train()
{
	// Convert score to reward as a PyObject
	PyObject *pyTrain = PyObject_CallMethod(pyaiInstance, "train", "(i)", player->getScore() - lastPlayerScore); // PyObject_CallMethodObjArgs faster?
	lastPlayerScore = player->getScore();

	if (pyTrain == NULL) {
		PyErr_Print();
		return;
	}
	Py_DECREF(pyTrain);
	return;
}

void PyAI::setPlayer(Player * player_)
{
	lastPlayerScore = player_->getScore();
	player = player_;
}

int PyAI::findAction()
{

	PyObject *pyActionID = PyObject_CallMethod(pyaiInstance, "doAction", NULL); // PyObject_CallMethodObjArgs faster?
	if (pyActionID == NULL) {
		PyErr_Print();
		return -1;
	}
	Py_DECREF(pyActionID);

	int actionID = PyLong_AsLong(pyActionID);
	return actionID;
}


bool PyAI::terminal()
{
	return false;
}

void PyAI::defineActionSpace()
{
}

void PyAI::updateState()
{
	int c = 0;
	for (auto &tile : game->map.tiles) {

		/// Tile
		//stateBuffer[c++] = (int)tile.id;// Layer
		//stateBuffer[c++] = (int)tile.tileID; // Layer
		//stateBuffer[c++] = (int)tile.oilYield;// Layer
		//stateBuffer[c++] = (int)tile.getResources();// Layer
		//stateBuffer[c++] = (int)tile.lumberYield;// Layer
		stateBuffer[c++] = (int)tile.walkable;// Layer
		stateBuffer[c++] = (int)tile.harvestable;// Layer
		//stateBuffer[c++] = (int)tile.swimable;// Layer


		Unit* occupant = tile.getOccupant();
		if (occupant) {
			// Unit
			//stateBuffer[c++] = (int)occupant->id;// Layer
			stateBuffer[c++] = (int)occupant->typeId;// Layer
			stateBuffer[c++] = (int)occupant->current_state;// Layer
			//stateBuffer[c++] = (int)occupant->goldCarry;// Layer
			//stateBuffer[c++] = (int)occupant->lumberCarry;// Layer
			//stateBuffer[c++] = (int)occupant->oilCarry;// Layer
			//stateBuffer[c++] = (int)occupant->carryCapacity;// Layer
			stateBuffer[c++] = (int)occupant->direction;// Layer
			//stateBuffer[c++] = (int)occupant->damageMax;// Layer
			//stateBuffer[c++] = (int)occupant->damageMin;// Layer
			//stateBuffer[c++] = (int)occupant->damagePiercing;// Layer
			//stateBuffer[c++] = (int)occupant->damageRange;// Layer
			stateBuffer[c++] = (int)occupant->health / (int)occupant->health_max; // Layer
			//stateBuffer[c++] = (int)occupant->health_max;//health_max;// Layer  // TODO
			//stateBuffer[c++] = (int)occupant->military;// Layer
			//stateBuffer[c++] = (int)occupant->recallable;// Layer
			//stateBuffer[c++] = (int)occupant->sight;// Layer
			//stateBuffer[c++] = (int)occupant->structure;// Layer

			// Player
			stateBuffer[c++] = (int)occupant->player_->id_;// Layer
			stateBuffer[c++] = (int)occupant->player_->faction;// Layer

		}
		else {
			//stateBuffer[c++] = 0;
			stateBuffer[c++] = 0;
			stateBuffer[c++] = 0;
			//stateBuffer[c++] = 0;
			//stateBuffer[c++] = 0;
			//stateBuffer[c++] = 0;
			//stateBuffer[c++] = 0;
			stateBuffer[c++] = 0;
			//stateBuffer[c++] = 0;
			//stateBuffer[c++] = 0;
			//stateBuffer[c++] = 0;
			//stateBuffer[c++] = 0;
			stateBuffer[c++] = 0;
			//stateBuffer[c++] = 0;
			//stateBuffer[c++] = 0;
			//stateBuffer[c++] = 0;
			//stateBuffer[c++] = 0;
			//stateBuffer[c++] = 0;

			// Player
			stateBuffer[c++] = 0;
			stateBuffer[c++] = 0;
		}

	}



	PyObject *byteArr;

	byteArr = PyByteArray_FromStringAndSize(stateBuffer, stateBufferSize);
	PyObject_SetAttrString(pyaiInstance, "state_array", byteArr);
	PyObject_SetAttrString(pyaiInstance, "state_rows", PyLong_FromLong(G_ROW));
	PyObject_SetAttrString(pyaiInstance, "state_cols", PyLong_FromLong(G_COL));
	PyObject_SetAttrString(pyaiInstance, "state_depth", PyLong_FromLong(G_FEATURES));

	Py_DECREF(byteArr); // TODO should this be done?
}

/*void PyAI::getStateBytes() {

}*/

PyObject* PyAI::registry_hook(PyObject* self, PyObject* args)
{
	PyObject *pyAI;
	if (!PyArg_UnpackTuple(args, "", 1, 1, &pyAI))
	{
		PyErr_Print();
		fprintf(stderr, "Failed to start as you are not inserting correct parameters (hook(PyAI))  (hook(self))");
		return PyLong_FromLongLong(-1);
	}

	PyAI *api = new PyAI(pyAI);
	AIRepository::getInstance().addAI(api);

	intptr_t ptr = reinterpret_cast<intptr_t>(api);
	return PyLong_FromLongLong(ptr);
}







static struct PyMethodDef methods[] = {
	{ "hook", PyAI::registry_hook, METH_VARARGS, "Hook ai to C++ endpoint" },
	{ NULL, NULL, 0, NULL }
};

static struct PyModuleDef modDef = {
	PyModuleDef_HEAD_INIT, "PyAPIRegistry", NULL, -1, methods,
	NULL, NULL, NULL, NULL
};

PyObject* PyAI::PyInit_PyAPIRegistry(void)
{
	return PyModule_Create(&modDef);
}
