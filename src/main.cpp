
#include "Config.h"
#include "Game.h"
#include "graphics/PyGUI.h"

int main() {
    Config config = Config::defaults();

    auto g = Game("15x15-2v2.json", config);
    Player &player0 = g.addPlayer();
    Player &player1 = g.addPlayer();
    g.init();
    auto gui = PyGUI(g);







    g.setMaxFPS(1000000000);
    g.setMaxUPS(1000000000);
    g.start();

    while(true){
        g.tick();
        g.update();
        g.render();
        g.caption();
        //gui.view();


        player0.do_action(rand() % 14);


        player1.do_action(rand() % 14);


    }
}





