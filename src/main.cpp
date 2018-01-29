

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifdef LINUX
#include <unistd.h>
#endif
#ifdef WINDOWS
#include <windows.h>
#endif

#include "Game.h"

#ifdef __linux__
void ensureDisplay() {
    if (!std::getenv("DISPLAY")) {
        std::cout << "Failed to open X11 display. Force-Disabling Display for this session" << std::endl;
        Config::getInstance().forceDisplay(false);

    }

}
#endif



int main() {
#ifdef __linux__
    ensureDisplay();
#endif

    // Create game instance
    Game *g = new Game();

    Player &player0 = g->addPlayer();
    Player &player1 = g->addPlayer();

    g->setMaxFPS(5);

    g->setMaxUPS(99999999999999999999999999);
    g->start();

    while(true) {

        g->tick();
        g->render();
        g->update();
        g->caption();


    }




}
