#include "graphics/GUIGame.h"

int main() {

    // Create game instance
    GUIGame *g = new GUIGame();

    Player& player0 = g->addPlayer();
    Player& player1 = g->addPlayer();

    Player& player2 = g->players[0];




    //g->setMaxFPS(5);

    g->setMaxUPS(99999999999999999999999999);
    g->start();
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
