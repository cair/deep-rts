

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
#include "game/algorithms/MCAS/MCAS.h"
#include "game/algorithms/PYAPI/PyAI.h"
#include "game/algorithms/base/AIRepository.h"
#include "game/algorithms/MCTSDirect/MCTSDirect.h"


#ifdef __linux__
void ensureDisplay() {
    if (!std::getenv("DISPLAY")) {
        std::cout << "Failed to open X11 display. Force-Disabling Display for this session" << std::endl;
        Config::getInstance().forceDisplay(false);

    }

}
#endif


#include <uWS/uWS.h>
int main() {
#ifdef __linux__
    ensureDisplay();
#endif



    /*
    uWS::Hub h;
    h.onMessage([](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
        ws->send(message, length, opCode);
    });


    std::cout << "lel lets go" << std::endl;
    h.listen(3000);
    h.run();*/


    // Create game instance
    Game *g = new Game(4, true);
    Player &player0 = g->addPlayer();
    Player &player1 = g->addPlayer();
    Player &player2 = g->addPlayer();
    Player &player3 = g->addPlayer();
    g->start();


    PyAI::start(); // Init the API

    Algorithm * algorithm0 = AIRepository::getInstance().getAI("DQN");
    player0.setAlgorithm(algorithm0);

    Algorithm *algorithm1 = new MCTS(&player1);
    player1.setAlgorithm(algorithm1);

    MCTSDirect *algorithm2 = new MCTSDirect(&player2);
    player2.setAlgorithm(algorithm2);

    Algorithm *algorithm3 = new MCAS(&player3);
    player3.setAlgorithm(algorithm3);


    g->initGUI();
    g->loop();


}
