//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_CONFIG_H
#define WARC2SIM_CONFIG_H

#include <fstream>
#include "loaders/MapLoader.h"

class Config
{
public:
    static Config& getInstance()
    {
        static Config    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

	uint32_t getFPS() {
        return fps;
    }

	uint32_t getUPS() {
        return ups;
    }

    double_t getFPSInterval() {
        return 1000.0 / getFPS();
    }

	double_t getUPSInterval() {
        return 1000.0 / getUPS();
    }

    bool getMechanicTownHall(){
        return mechanicTownHall;
    }

	uint8_t getTickModifier() {
        return tickModifier;
    }

	bool getCaptionConsole() {
		return captionConsole;
	}

	bool getCaptionWindow() {
		return captionWindow;
	}

    bool getMechanicHarvestReplay() {
        return mechanicHarvestForever;
    }
    static void print(std::string data) {
        std::cout << data << std::endl;
    }

	const bool getJsonLogging() {
		return jsonLogging;
	}




private:
    Config() {

        // Load configuration
        load();
    }                    // Constructor? (the {} brackets) are needed here.


    int fps;
    int ups;
    bool mechanicTownHall;
    int tickModifier;
	bool captionConsole;
	bool captionWindow;
	bool mechanicHarvestForever;
	bool jsonLogging;

    void load(){
        // Read Map data
        std::ifstream confData("./data/config.json");
        json configuration;
        confData >> configuration;

        json mechanics = configuration["mechanics"];
		json logging = configuration["logging"];

        fps = configuration["fps"];
        ups = configuration["ups"];
        tickModifier = configuration["ticks_per_sec"];
        mechanicTownHall = mechanics["town_hall_on_start"];
		mechanicHarvestForever = mechanics["harvest_forever"];
		captionConsole = configuration["caption"]["console"];
		captionWindow = configuration["caption"]["window"];
		jsonLogging = logging["json_scorings"];



    }

    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
public:
    Config(Config const&)               = delete;
    void operator=(Config const&)  = delete;

    // Note: Scott Meyers mentions in his Effective Modern
    //       C++ book, that deleted functions should generally
    //       be public as it results in better error messages
    //       due to the compilers behavior to check accessibility
    //       before deleted status


};
#endif //WARC2SIM_CONFIG_H
