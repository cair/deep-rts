//
// Created by Per-Arne on 23.02.2017.
//

#ifndef WARC2SIM_GAME_H
#define WARC2SIM_GAME_H

#include <memory>
#include <map>
#include <unordered_map>
#include <iostream>
#include <chrono>

#include "util/matrix.cpp"
#include "player/Player.h"
#include "environment/Map.h"
#include "environment/Tilemap.h"
#include "Config.h"

class Game {

    /// List of Game instances
    static std::unordered_map<int, Game *> games;

    /// Game Clock
    std::chrono::high_resolution_clock::time_point now;
    std::chrono::high_resolution_clock::time_point _update_next;
    std::chrono::high_resolution_clock::time_point _render_next;
    std::chrono::high_resolution_clock::time_point _stats_next;

    std::chrono::nanoseconds _update_interval;
    std::chrono::nanoseconds _render_interval;

    uint32_t _update_delta = 0;
    uint32_t _render_delta = 0;

    /// Spawn a player
    void spawnPlayer(Player & player);

    /// Initialize the game clock timers
    void timerInit();

    ///
    bool running;



public:
    /// Init the constructor
    void init();

    // Retrieve game via Game ID
    static Game * getGame(uint8_t id);

    /// Game Constructor
    explicit Game(std::string map_file);

    Game(std::string map_file, Config config);


    ////////////////////////////////////////////
    ///
    /// Properties
    ///
    ////////////////////////////////////////////////////
    /// Const Config (Order 0)
    const Config config;

    /// Const Map (Order 1)
    Map map;

    /// Game state (Order 2)
    cppmat::matrix<float> state;

    /// Game Tilemap (Order 3)
    Tilemap tilemap;

    /// Game State Manager
    StateManager stateManager;

    /// List of Players inside the game session
    std::vector<Player> players;

    /// List of Units inside the game session
    std::vector<Unit> units;

    /// Game Identification
    uint8_t id;

    /// Game Episode Ticks
    uint64_t ticks = 0;

    /// Game Episode
    uint16_t episode = 1;

    /// Game Max FPS
	uint32_t max_fps;

    /// Game Max UPS
	uint32_t max_ups;

    /// Game Current FPS
    uint32_t currentFPS = 0;

    /// Game Current UPS
    uint32_t currentUPS = 0;

    /// Game terminal flag
    bool terminal = false;

    /// Selected player
    Player* selectedPlayer;

    ////////////////////////////////////////////////////
    ///
    /// Getters
    ///
    ////////////////////////////////////////////////////

    /// Get a Unit via index
    Unit &getUnit(uint16_t idx);

    uint32_t getMaxFPS() const;

    uint32_t getMaxUPS() const;

    uint32_t getFPS() const;

    uint32_t getUPS() const;

    uint64_t getGameDuration() const;

    uint64_t getTicks() const ;

    size_t getWidth() const;

    size_t getHeight() const;

    uint16_t getEpisode() const;

    uint8_t getId() const;

    int getTicksModifier() const;

    ////////////////////////////////////////////////////
    ///
    /// Setters
    ///
    ////////////////////////////////////////////////////

    /// Set the Game FPS
    void setMaxFPS(uint32_t fps_);

    /// Set the Game UPS
    void setMaxUPS(uint32_t ups_);

    /// Set selected player
    void setSelectedPlayer(Player &player);


    ////////////////////////////////////////////////////
    ///
    /// Engine Functions
    ///
    ////////////////////////////////////////////////////
    // Add a player to the game
    Player &addPlayer();

    /// Game Tick Function
    void tick();

    /// Game Update Function
    void update();
    virtual void _update();

    /// Game Render Function
    void render();

    /// Game Render implementation
    virtual void _render();

    /// Print the Game Statistics
    void caption();

    virtual void _caption();

    // Enables the running flag and updates the clock
    void start();

    // Disables the running flag
    void stop();

    // Resets the game
    void reset();
    virtual void _reset();

    // Returns true if the game is in an terminal state
    bool isTerminal();

    ////////////////////////////////////////////////////
    ///
    /// Callback Functions
    ///
    ////////////////////////////////////////////////////
    // Unit Events
    virtual void _onUnitCreate(Unit& unit);
    virtual void _onUnitDestroy(Unit& unit);

    // Resource Events
    virtual void _onResourceGather(Tile& tile, Unit& unit);
    virtual void _onResourceDepleted(Tile& tile, Unit& unit);


    virtual void _onEpisodeStart();
    virtual void _onEpisodeEnd();

    virtual void _onTileChange(Tile &);
};


#endif //WARC2SIM_GAME_H
