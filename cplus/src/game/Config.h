//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_CONFIG_H
#define WARC2SIM_CONFIG_H

#include <fstream>
#include <rapidjson/istreamwrapper.h>
#include <cmath>
#include "loaders/ResourceLoader.h"

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

	uint32_t getAPM() {
		return apm;
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

	const bool getLoggingScoring() {
		return loggingScoring;
	}

	const bool getDisplay() {
		return display;
	}

	void forceDisplay(bool val) {
		display = val;
	}

	const int getTickReset() {
		return tickReset;
	}

	const std::string getMapName() {
		return mapName;
	}

	int32_t getAudioVolume() {
		return audioVolume;
	}

	void setAudioVolume(int32_t _audioVolume) {
		audioVolume = _audioVolume;
	}

	bool getAudioEnabled() {
		return audioEnabled;
	}

	bool mechanicFood;
	bool mechanicFarm;
	bool mechanicBarracks;
	bool mechanicFootman;
	bool mechanicArcher;
    bool mechanicInstantBuild;
	bool mechanicFOW;



private:
    Config() {

        // Load configuration
        load();
    }                    // Constructor? (the {} brackets) are needed here.


    int fps;
    int ups;
	int apm;
	int tickReset;
    bool mechanicTownHall;
    int tickModifier;
	bool captionConsole;
	bool captionWindow;
	bool mechanicHarvestForever;
	bool loggingScoring;
	bool display;
	int32_t audioVolume;
	bool audioEnabled;
	std::string mapName;




    void load(){
        // Read Map data
        auto conf = ResourceLoader::getInstance().configJSON.GetObject();

        auto mechanics = conf["mechanics"].GetObject();
		auto logging = conf["logging"].GetObject();
		auto ai = conf["ai"].GetObject();
		auto audio = conf["audio"].GetObject();

		tickReset = conf["tick_reset"].GetInt();
		apm = ai["max_apm"].GetInt();
        fps = conf["fps"].GetInt();
        ups = conf["ups"].GetInt();
		mapName = conf["map"].GetString();
        tickModifier = conf["ticks_per_sec"].GetInt();
		captionConsole = conf["caption"]["console"].GetBool();
		captionWindow = conf["caption"]["window"].GetBool();
		loggingScoring = logging["scorings"].GetBool();
		display = conf["display"].GetBool();

		mechanicTownHall = mechanics["town_hall_on_start"].GetBool();
		mechanicHarvestForever = mechanics["harvest_forever"].GetBool();

		mechanicArcher = mechanics["archer"].GetBool();
		mechanicBarracks = mechanics["barracks"].GetBool();
		mechanicFarm = mechanics["farm"].GetBool();
		mechanicFootman = mechanics["footman"].GetBool();
		mechanicFood = mechanics["food"].GetBool();
		mechanicInstantBuild = mechanics["instant_build"].GetBool();
		mechanicFOW = mechanics["fog_of_war"].GetBool();

		audioEnabled = audio["enabled"].GetBool();
		audioVolume = audio["volume"].GetInt();

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
