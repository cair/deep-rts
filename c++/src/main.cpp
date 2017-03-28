

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
#include "game/algorithms/PYAPI/PyAI.h"
#include "game/algorithms/base/AIRepository.h"

int main() {
	
	// Create game instance
    Game *g = new Game(4, true);
    Player &player0 = g->addPlayer();
    Player &player1 = g->addPlayer();
    Player &player2 = g->addPlayer();
    Player &player3 = g->addPlayer();
    g->start();

//	PyAI::start(); // Init the API


	
    Algorithm *algorithm0 = new MCTS(&player0);
    player0.setAlgorithm(algorithm0);

	AlgoRandom *algorithm1 = new AlgoRandom(&player1);
//    Algorithm * algorithm1 = AIRepository::getInstance().getAI("DQN");
    player1.setAlgorithm(algorithm1);

	AlgoRandom *algorithm2 = new AlgoRandom(&player2);
    player2.setAlgorithm(algorithm2);

    AlgoRandom *algorithm3 = new AlgoRandom(&player3);
    player3.setAlgorithm(algorithm3);


	//CRemoteAI ai = CRemoteAI::createInstance(g->id);

    g->initGUI();
    g->loop();
	

}
