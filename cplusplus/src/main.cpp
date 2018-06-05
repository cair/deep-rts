
#include "Config.h"
#include "Game.h"
#include "graphics/PyGUI.h"
//#include "graphics/GUIGame.h"


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

    /*
    Config config = Config::defaults();

    // Create game instance
    Game *g = new Game("15x15-2v2.json", config);

    Player &player0 = g->addPlayer();
    Player &player1 = g->addPlayer();

    Player &player2 = g->players[0];


    g->setMaxFPS(10000000000000);
    g->setMaxUPS(10000000000000);
    g->start();
    srand(time(NULL));
    while (true) {
        while (!g->terminal) {
            g->tick();
            g->update();
            g->render();
            g->caption();

            player0.do_action(rand() % 16 + 1);
            player1.do_action(rand() % 16 + 1);
        }
        g->reset();


    }*/
}





