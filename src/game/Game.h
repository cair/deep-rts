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



class GameMessage;
class GUI;
class Game {

    int n_players;

    // Gameclock variables
    clock_t _update_next;
    clock_t _render_next;
    clock_t _stats_next;

    int _update_interval = 0;
    int _render_interval = 0;
    int _caption_interval = 1000;

    int _update_delta = 0;
    int _render_delta = 0;


    bool running;
    long ticks = 0;
	static std::unordered_map<int, Game *> games;
public:
	
	static Game * getGame(int id);
	int id;

    Tilemap map;
    int fps;
    int ups;

    int getSeconds();
    long getFrames();

    StateManager stateManager;
    std::vector<std::shared_ptr<Player>> players;
    std::vector<std::shared_ptr<BaseAction>> executedActions;
    void addAction(std::shared_ptr<BaseAction> action);


    void loop();
    void update(int ticks);
    void render(int ticks);
    void createPlayers();
    void load_players();


    void start();

    void stop();

    Game(int n_players, bool setup);

    GUI *gui;

    void initGUI();

    void setFPS(int fps_);

    void setUPS(int ups_);

    int currentFPS = 0;
    int currentUPS = 0;

    bool checkTerminal();

    bool terminal;

    Player &addPlayer();


	void deactivateGUI();
	GameMessage serialize();
	std::string serialize_json();
	void load(GameMessage& gameMessage);


};


#endif //WARC2SIM_GAME_H
