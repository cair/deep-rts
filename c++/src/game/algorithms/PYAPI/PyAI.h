
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif


#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <map>
#include "../../algorithms/base/Algorithm.h"




class Player;
class Game;
class PyAI: public Algorithm {
public:
	Game *game;
	char *stateBuffer;
	int stateBufferSize;
	int G_ROW;
	int G_COL;
	int G_FEATURES;

	int lastPlayerScore = 0;


	static void start();

	PyObject *pyaiInstance;
	PyAI(PyObject *pyaiInstance);
	std::string scriptName;

	void setPlayer(Player *player);
	virtual int findAction();
	virtual void update();
	virtual void reset();
	virtual void train();
	virtual bool terminal();
	virtual void defineActionSpace();

	void updateState();

	static PyObject *registry_hook(PyObject* self, PyObject* args);
	static PyObject* PyInit_PyAPIRegistry(void);

};



