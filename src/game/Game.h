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



class GUI;
class Game {

    uint8_t n_players;

    // Gameclock variables
    clock_t _update_next;
    clock_t _render_next;
    clock_t _stats_next;

    clock_t _update_interval = 0;
    clock_t _render_interval = 0;
    clock_t _caption_interval = 1000;

	uint32_t _update_delta = 0;
	uint32_t _render_delta = 0;


    bool running;
    long ticks = 0;
	static std::unordered_map<int, Game *> games;
public:
	
	static Game * getGame(uint8_t id);
	uint8_t id;

    Tilemap map;
	uint32_t fps;
	uint32_t ups;

	uint64_t getSeconds();
	uint64_t getFrames();

    StateManager stateManager;
    std::vector<std::shared_ptr<Player>> players;
    std::vector<std::shared_ptr<BaseAction>> executedActions;
    void addAction(std::shared_ptr<BaseAction> action);


    void loop();
	Tilemap & getMap();
    void update(uint32_t ticks);
    void render(uint32_t ticks);
    void createPlayers();
    void load_players();


    void start();

    void stop();

    Game(uint8_t n_players, bool setup);

    GUI *gui;

    void initGUI();

    void setFPS(uint32_t fps_);

    void setUPS(uint32_t ups_);

	uint32_t currentFPS = 0;
	uint32_t currentUPS = 0;

    bool checkTerminal();

    bool terminal;

    Player &addPlayer();

	std::string serialize_json();


	void deactivateGUI();
	


};


#endif //WARC2SIM_GAME_H
