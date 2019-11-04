
#include "Config.h"
#include "Game.h"
#include "graphics/PyGUI.h"

int main() {
    Config config = Config::defaults();

    Game *g = new Game("15x15-2v2.json", config);
    Player &player0 = g->addPlayer();
    Player &player1 = g->addPlayer();
    auto gui = PyGUI(*g);
    g->init();





    g->setMaxFPS(1000000000);
    g->setMaxUPS(1000000000);
    g->start();

    while(true){
        g->tick();
        g->update();
        g->render();
        g->caption();
        gui.render();
        gui.view();

        auto v1 = rand() % 15;
        player0.do_action(v1);

        auto v2 = rand() % 42;
        player1.do_action(v2);

        if(g->isTerminal()){
            g->reset();
        }
    }
}





