#include "graphics/GUIGame.h"
#include <string>
int main() {

    // Create game instance
    GUIGame *g = new GUIGame("small-2v2.json");
    g->tickModifier = 1;
    g->setMaxFPS(60);
    g->setMaxUPS(10);
    g->start();



    Player& player0 = g->addPlayer();
    Player& player1 = g->addPlayer();

    Player& player2 = g->players[0];







    srand (time(NULL));
    while(true){
        while(!g->terminal){
            g->tick();
            g->render();
            g->update();
            g->caption();
            //player0.queueActionA(rand() % 16 + 1, 1);
            //player1.queueActionA(rand() % 16 + 1, 1);
        }
        g->reset();





    }




}
