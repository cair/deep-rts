
#ifdef _DEBUG
#undef _DEBUG
#include <python.h>
#define _DEBUG
#else
#include <python.h>
#endif
#include <string>

#include <iostream>
#include <vector>
#include <map>
#include <array>
#include <memory>

#include "../../algorithms/base/Algorithm.h"
#include "../../serialization/Game_generated.h"




class Player;
class Game;
class PyAPI: public Algorithm {
public:
	Game *game;
	int *flatStateBuffer;
	int flatStateBufferSize;
	int G_ROW;
	int G_COL;
	int G_FEATURES;


	static bool loaded;

	static void init();


	
	static void start();

	PyAPI(uint8_t gameID, uint8_t playerID);
	std::string scriptName;


	virtual void update();
	virtual void terminal();
	virtual void defineActionSpace();
	virtual std::shared_ptr<BaseAction> findBestAction(std::shared_ptr<Unit> unit);
	virtual void doAction(std::shared_ptr<BaseAction> action);

	static PyObject * registry_loaded(PyObject * self, PyObject * args);
	static PyObject* registry_free(PyObject* self, PyObject* args);
	static PyObject* registry_hook(PyObject* self, PyObject* args);
	static PyObject* registry_get_state(PyObject* self, PyObject* args);

	static PyObject* PyInit_PyAPIRegistry(void);
};



