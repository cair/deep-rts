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
#include "logging/LogGame.h"
#include <SFML/System/Clock.hpp>

class GUI;
class Game {
	bool doCaptionWindow;
	bool doCaptionConsole;

	bool doScoreLogging;
    uint16_t gameNum = 1;

    // Gameclock variables
	sf::Clock clock;
    uint64_t _update_next;
	uint64_t _render_next;
	uint64_t _stats_next;
	double_t _apm_next;

    int32_t _update_interval = 0;
	int32_t _render_interval = 0;
	double_t _apm_interval = 0;

	uint32_t _update_delta = 0;
	uint32_t _render_delta = 0;

	LogGame gameLog; // Only in use when scoreLog flag s set in config
    bool running;
	bool triggerReset;
    long ticks = 0;
	static std::unordered_map<int, Game *> games;
public:
	bool doDisplay; // TODO?
	bool loaded = false;
	static Game * getGame(uint8_t id);
	uint8_t id;

    Tilemap map;
	uint32_t fps;
	uint32_t ups;
	uint32_t apm;
	uint32_t tickReset;

	uint64_t getSeconds();
	uint64_t getFrames();
	uint32_t getGameCount();


    StateManager stateManager;
    std::vector<Player> players;
	std::vector<Unit> units;

    std::vector<std::shared_ptr<BaseAction>> executedActions;
    void addAction(std::shared_ptr<BaseAction> action);


    void loop();
    void update();
    void render();

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
	void setAPM(uint16_t apm_);
	void triggerResetNow();

	uint32_t currentFPS = 0;
	uint32_t currentUPS = 0;

    bool checkTerminal();

    bool terminal;

    Player &addPlayer();
	void spawnPlayer(Player & player);
	Unit &getUnit(uint16_t idx);



	void load(Game *other);


	void deactivateGUI();


    uint8_t n_players;

    void tick();
    sf::Int64 nowMicroSec;

    void caption();

    void timerInit();
};


#endif //WARC2SIM_GAME_H
