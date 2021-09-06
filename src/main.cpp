

#include "Config.h"
#include "Game.h"
#include <effolkronium/random.hpp>
#include <opencv2/highgui.hpp>
#include "scenario/scenarios/GoldCollectFifteen.h"

using Random = effolkronium::random_static;

int main() {
    //auto webServer = Webserver("0.0.0.0", 4300, true);
    //webServer.start();
    Config config = Config::defaults();
    config.setConsoleCaptionEnabled(true);
    config.setGUI("");

//    auto g = Game("10x10-2v2.json", config);

    auto g = DeepRTS::Scenario::GoldCollectFifteen();

    Player &player0 = g.addPlayer();
    Player &player1 = g.addPlayer();

    g.setMaxFPS(0);
    g.start();

    int EPISODES = 100000;

    g.getUnitByNameID("Test1");


    for(auto episode=0; episode < EPISODES; episode++)
    {

        while(!g.isTerminal())
        {

            player0.do_action(Random::get(Constants::ACTION_MIN, Constants::ACTION_MAX));
            player1.do_action(Random::get(Constants::ACTION_MIN, Constants::ACTION_MAX));

            g.update();
            auto& image = g.render();
            g.caption();


            cv::imshow("Title", image);
            cv::waitKey(1);



        }

        g.reset();


    }
}





