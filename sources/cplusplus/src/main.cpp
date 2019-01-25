
#include "Config.h"
#include "Game.h"
#include "graphics/PyGUI.h"

int main() {
    Config config = Config::defaults();

    Game *g = new Game("15x15-2v2.json", config);
    auto gui = PyGUI(*g);


    Player &player0 = g->addPlayer();
    Player &player1 = g->addPlayer();

    Player &player2 = g->players[0];


    g->setMaxFPS(10000000000000);
    g->setMaxUPS(10000000000000);
    g->start();

    while(true){
        g->tick();
        g->update();
        g->render();
        g->caption();
        gui.render();
        gui.view();
    }
}





