

#include <thread>


#include "game/Game.h"
#include "server/Server.h"
#include "game/algorithms/RANDOM/AlgoRandom.h"


void startServer(Game *game){

    Server *s = new Server(game);


}


int main() {


    Game *g = new Game(4);
    Player &player0 = g->addPlayer();
    Player &player1 = g->addPlayer();
    Player &player2 = g->addPlayer();
    Player &player3 = g->addPlayer();


    g->start();

    std::shared_ptr<AlgoRandom> algorithm0 = std::shared_ptr<AlgoRandom>(new AlgoRandom(player0));
    //player0.setAlgorithm(algorithm0);

    std::shared_ptr<AlgoRandom> algorithm1 = std::shared_ptr<AlgoRandom>(new AlgoRandom(player1));
    //player1.setAlgorithm(algorithm1);

    std::shared_ptr<AlgoRandom> algorithm2 = std::shared_ptr<AlgoRandom>(new AlgoRandom(player2));
    //player2.setAlgorithm(algorithm2);

    std::shared_ptr<AlgoRandom> algorithm3 = std::shared_ptr<AlgoRandom>(new AlgoRandom(player3));
    //player3.setAlgorithm(algorithm3);

    g->initGUI();
    g->loop();



}