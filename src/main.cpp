

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifdef LINUX
#include <unistd.h>
#endif
#ifdef WINDOWS
#include <windows.h>
#endif

void sleep(int sleepMs)
{
#ifdef LINUX
	usleep(sleepMs * 1000);   // usleep takes sleep time in us (1 millionth of a second)
#endif
#ifdef WINDOWS
	Sleep(sleepMs);
#endif
}


#include "game/Game.h"
#include "game/algorithms/RANDOM/AlgoRandom.h"
#include "game/algorithms/MCTS/MCTS.h"
//#include "game/algorithms/REMOTEAI/zmqAI.h"
#include "game/algorithms/PYAPI/PyAPI.h"

int main() {
	
	// Create game instance
    Game *g = new Game(4, true);
    Player &player0 = g->addPlayer();
    Player &player1 = g->addPlayer();
    Player &player2 = g->addPlayer();
    Player &player3 = g->addPlayer();
    g->start();

	PyAPI::init(); // Init the API

	volatile int x = 0;
	while (!PyAPI::loaded) { x++; x += PyAPI::loaded; x--; }


	//std::shared_ptr<zmqAI> ai = zmqAI::createInstance(0, 0);
    //player0.setAlgorithm(algorithm0);

    //std::shared_ptr<MCTS> algorithm1 = std::shared_ptr<MCTS>(new MCTS(player1));
	std::shared_ptr<AlgoRandom> algorithm1 = std::shared_ptr<AlgoRandom>(new AlgoRandom(player1));
	player1.setAlgorithm(algorithm1);

    std::shared_ptr<AlgoRandom> algorithm2 = std::shared_ptr<AlgoRandom>(new AlgoRandom(player2));
    player2.setAlgorithm(algorithm2);

    std::shared_ptr<AlgoRandom> algorithm3 = std::shared_ptr<AlgoRandom>(new AlgoRandom(player3));
    player3.setAlgorithm(algorithm3);


	//CRemoteAI ai = CRemoteAI::createInstance(g->id);

    g->initGUI();
    g->loop();
	

}
