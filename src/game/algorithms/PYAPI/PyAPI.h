
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




class Player;
class Game;
class PyAPI {
public:
	Game *game;
	std::shared_ptr<Player> player;

	static std::vector<PyAPI *> instances;
	static std::vector<PyAPI *> freeInstances;
	static std::array<PyAPI *, 16> slots;
	static int slotCounter;

	static void init();

	static void createInstance(std::string scriptName, uint16_t gameID, uint16_t playerID);

	void start();

	PyAPI(std::string scriptName, uint16_t gameID, uint16_t playerID);
	std::string scriptName;
	void world(PyObject *self, PyObject *args);




	static PyObject* registry_hook(PyObject* self, PyObject* args);
	static PyObject* registry_get_state(PyObject* self, PyObject* args);
	static PyObject* PyInit_PyAPIRegistry(void);


};



