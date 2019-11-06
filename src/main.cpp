
#include <random>
#include "Config.h"
#include "Game.h"



int main() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 16); // distribution in range [1, 6]

    Config config = Config::defaults();
    config.setConsoleCaptionEnabled(true);
    config.setGUI(true);

    auto g = Game("15x15-2v2.json", config);

    Player &player0 = g.addPlayer();
    Player &player1 = g.addPlayer();

    g.setMaxFPS(10000000);
    g.setMaxUPS(10000000);
    g.start();


    while(true){

        g.update();
        g.render();
        g.caption();

        player0.do_action(dist6(rng));
        player1.do_action(dist6(rng));

        if(g.isTerminal()) {

            g.reset();
        }


    }
}





