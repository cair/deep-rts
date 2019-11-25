
#include <random>
#include "Config.h"
#include "Game.h"
#include "util/Random.h"


int main() {

    Config config = Config::defaults();
    config.setConsoleCaptionEnabled(true);
    config.setGUI(true);

    auto g = Game("15x15-2v2.json", config);

    Player &player0 = g.addPlayer();
    Player &player1 = g.addPlayer();

    g.setMaxFPS(10000000);
    g.setMaxUPS(10000000);
    g.start();

    int EPISODES = 100000;

    g.getUnitByNameID("Test1");


    for(auto episode=0; episode < EPISODES; episode++)
    {

        while(!g.isTerminal())
        {
            g.update();
            g.render();
            g.caption();

            player0.do_action(Random::randInt(Constants::ACTION_MIN, Constants::ACTION_MAX));
            player1.do_action(Random::randInt(Constants::ACTION_MIN, Constants::ACTION_MAX));

        }

        g.reset();


    }
}





