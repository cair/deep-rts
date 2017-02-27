

#include "game/Game.h"


int main() {


    Game g = Game(4);
    g.start();
    g.create_players();
    g.initGUI();
    g.loop();



}