//
// Created by Per-Arne on 23.02.2017.
//

#ifndef WARC2SIM_GAME_H
#define WARC2SIM_GAME_H

#include <time.h>
#include <memory>
#include <map>
#include <unordered_map>

#include "player/Player.h"
#include "environment/Tilemap.h"
#include "action/BaseAction.h"
#include "./util/ScoreLogSerializer.h"
#include <SFML/System.hpp>


class GUI;
class Game {
	const bool doCaptionWindow;
	const bool doCaptionConsole;
	const bool doDisplay;
	const bool doScoreLogging;
    uint8_t n_players;
	uint16_t gameNum = 1;

    // Gameclock variables
	sf::Clock clock;
    uint64_t _update_next;
	uint64_t _render_next;
	uint64_t _stats_next;

    int32_t _update_interval = 0;
	int32_t _render_interval = 0;
	int32_t _caption_interval = 1000;

	uint32_t _update_delta = 0;
	uint32_t _render_delta = 0;

	ScoreLogSerializer scoreLog; // Only in use when scoreLog flag s set in config
    bool running;
	bool triggerReset;
    long ticks = 0;
	static std::unordered_map<int, Game *> games;
public:
	bool loaded = false;
	static Game * getGame(uint8_t id);
	uint8_t id;

    Tilemap map;
	uint32_t fps;
	uint32_t ups;

	uint64_t getSeconds();
	uint64_t getFrames();
	uint32_t getGameCount();


    StateManager stateManager;
    std::vector<Player> players;
	std::vector<Unit> units;

    std::vector<std::shared_ptr<BaseAction>> executedActions;
    void addAction(std::shared_ptr<BaseAction> action);


    void loop();
	Tilemap & getMap();
    void createPlayers();


    void start();
    void stop();
	void reset();

    Game(uint8_t n_players, bool setup);

    GUI *gui;

    void initGUI();

    void setFPS(uint32_t fps_);
    void setUPS(uint32_t ups_);
	void triggerResetNow();

	uint32_t currentFPS = 0;
	uint32_t currentUPS = 0;

    bool checkTerminal();

    bool terminal;

    Player &addPlayer();
	void spawnPlayer(Player & player);
	Unit &getUnit(uint16_t idx);

	std::string serialize_json();


	void deactivateGUI();
	


};


#endif //WARC2SIM_GAME_H
