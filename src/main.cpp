#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <thread>


#include "game/Game.h"
#include "game/algorithms/RANDOM/AlgoRandom.h"
#include "game/algorithms/MCTS/MCTS.h"

#include "game/algorithms/REMOTEAI/CRemoteAI.h"
#include "game/algorithms/REMOTEAI/zmqAI.h"
#include "game/third_party/zmq.hpp"

#include <zmq_utils.h>

int main() {
	// REMEMBER PY FILE MUST BE IN WORKING DIR (WITH EXE) Copyed by visual studio
	/*CRemoteAI::PySetupImport();					// Pre-import C++ API endpoint
	Py_Initialize();							// Init Python interp
	pybind11::init();							// Init Pybind interp
	CRemoteAI::PyRunImport();					// Run actual imports*/
	//  Prepare our context and socket



	// Create game instance
    Game *g = new Game(4, true);
    Player &player0 = g->addPlayer();
    Player &player1 = g->addPlayer();
    Player &player2 = g->addPlayer();
    Player &player3 = g->addPlayer();


    g->start();

	std::shared_ptr<zmqAI> ai = zmqAI::createInstance(1, 0);

    std::shared_ptr<AlgoRandom> algorithm0 = std::shared_ptr<AlgoRandom>(new AlgoRandom(player0));
    //player0.setAlgorithm(algorithm0);

    std::shared_ptr<MCTS> algorithm1 = std::shared_ptr<MCTS>(new MCTS(player1));
    //player1.setAlgorithm(algorithm1);

    std::shared_ptr<AlgoRandom> algorithm2 = std::shared_ptr<AlgoRandom>(new AlgoRandom(player2));
    //player2.setAlgorithm(algorithm2);

    std::shared_ptr<AlgoRandom> algorithm3 = std::shared_ptr<AlgoRandom>(new AlgoRandom(player3));
    //player3.setAlgorithm(algorithm3);


	//CRemoteAI ai = CRemoteAI::createInstance(g->id);

    g->initGUI();
    g->loop();

}