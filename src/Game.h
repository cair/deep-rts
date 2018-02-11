//
// Created by Per-Arne on 23.02.2017.
//

#ifndef WARC2SIM_GAME_H
#define WARC2SIM_GAME_H
#include "../include/cppmat/src/cppmat/cppmat.h"

#include <memory>
#include <map>
#include <unordered_map>
#include <iostream>
#include <chrono>


#include "player/Player.h"
#include "environment/Map.h"
#include "environment/Tilemap.h"


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


    bool running;

protected:
    /// Game Constructor
    Game(std::string map_file);


public:
    // Retrieve game via Game ID
    static Game * getGame(uint8_t id);

    static Game create(std::string map_file){
        auto g = Game(map_file);

    }

    int tickModifier = 10;
    bool instantTownHall = true;
    bool instantBuilding = true;
    bool harvestForever = false;
    bool autoAttack = true;
    int foodLimit = 100;
    bool enableFarm = true;
    bool enableBarracks = true;
    bool enableFootman = true;
    bool enableArcher = false;
    bool enableAudio = false;
    bool audioVolume = 0;

    void setTickModifier(int tickmodifier){
        tickModifier = tickmodifier;
    }

    void setInstantTownHall(bool b){
        instantTownHall = b;
    }

    void setInstantBuilding(bool b){
        instantBuilding = b;
    }

    void setHarvestForever(bool b){
        harvestForever = b;
    }

    void setAutoAttack(bool b){
        autoAttack = b;
    }

    void setFoodLimit(bool b){
        foodLimit = b;
    }

    void setFarm(bool b){
        enableFarm = b;
    }

    void setBarracks(bool b){
        enableBarracks = b;
    }

    void setFootman(bool b){
        enableFootman = b;
    }

    void setArcher(bool b){
        enableArcher = b;
    }

    void setAudio(bool b, int volume){
        enableAudio = b;
        audioVolume = volume;
    }
    static Game create(std::string map_file, int tick_modifier){
        auto g = Game(std::move(map_file));
        g.setTickModifier(tick_modifier);
        g.setInstantTownHall(true);
        g.setInstantBuilding(true);
        g.setHarvestForever(false);
        g.setAutoAttack(true);
        g.setFoodLimit(100);
        g.setFarm(true);
        g.setBarracks(true);
        g.setFootman(true);
        g.setArcher(false);
        return g;

    }
    static Game create(std::string map_file, int tick_modifier, bool instant_town_hall){
        auto g = Game(std::move(map_file));
        g.setTickModifier(tick_modifier);
        g.setInstantTownHall(instant_town_hall);
        g.setInstantBuilding(true);
        g.setHarvestForever(false);
        g.setAutoAttack(true);
        g.setFoodLimit(100);
        g.setFarm(true);
        g.setBarracks(true);
        g.setFootman(true);
        g.setArcher(false);
        return g;
    }
    static Game create(std::string map_file, int tick_modifier, bool instant_town_hall, bool instant_building){
        auto g = Game(std::move(map_file));
        g.setTickModifier(tick_modifier);
        g.setInstantTownHall(instant_town_hall);
        g.setInstantBuilding(instant_building);
        g.setHarvestForever(false);
        g.setAutoAttack(true);
        g.setFoodLimit(100);
        g.setFarm(true);
        g.setBarracks(true);
        g.setFootman(true);
        g.setArcher(false);
        return g;
    }
    static Game create(std::string map_file, int tick_modifier, bool instant_town_hall, bool instant_building, bool harvest_forever){
        auto g = Game(std::move(map_file));
        g.setTickModifier(tick_modifier);
        g.setInstantTownHall(instant_town_hall);
        g.setInstantBuilding(instant_building);
        g.setHarvestForever(harvest_forever);
        g.setAutoAttack(true);
        g.setFoodLimit(100);
        g.setFarm(true);
        g.setBarracks(true);
        g.setFootman(true);
        g.setArcher(false);
        return g;
    }
    static Game create(std::string map_file, int tick_modifier, bool instant_town_hall, bool instant_building, bool harvest_forever, bool food_limit){
        auto g = Game(std::move(map_file));
        g.setTickModifier(tick_modifier);
        g.setInstantTownHall(instant_town_hall);
        g.setInstantBuilding(instant_building);
        g.setHarvestForever(harvest_forever);
        g.setAutoAttack(true);
        g.setFoodLimit(food_limit);
        g.setFarm(true);
        g.setBarracks(true);
        g.setFootman(true);
        g.setArcher(false);
        return g;
    }
    static Game create(std::string map_file, int tick_modifier, bool instant_town_hall, bool instant_building, bool harvest_forever, bool auto_attack, bool food_limit, bool farm, bool barracks, bool footman, bool archer){
        auto g = Game(std::move(map_file));
        g.setTickModifier(tick_modifier);
        g.setInstantTownHall(instant_town_hall);
        g.setInstantBuilding(instant_building);
        g.setHarvestForever(harvest_forever);
        g.setAutoAttack(auto_attack);
        g.setFoodLimit(food_limit);
        g.setFarm(farm);
        g.setBarracks(barracks);
        g.setFootman(footman);
        g.setArcher(archer);
        return g;
    }

    ////////////////////////////////////////////////////
    ///
    /// Properties
    ///
    ////////////////////////////////////////////////////
    /// Const Map (Order 0)
    Map map;

    /// Game state (Order 1)
    cppmat::matrix<float> state;

    /// Game Tilemap (Order 2)
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

    /// Bool that determine print of FPS and UPS in console
    bool consoleCaptionEnabled = true;

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

    uint8_t getTicksModifier() const;

    ////////////////////////////////////////////////////
    ///
    /// Setters
    ///
    ////////////////////////////////////////////////////

    /// Set the Game FPS
    void setMaxFPS(uint32_t fps_);

    /// Set the Game UPS
    void setMaxUPS(uint32_t ups_);


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
    /// Engine Functions
    ///
    ////////////////////////////////////////////////////
    virtual void _onUnitCreate(Unit& unit);
    virtual void _onUnitDestroy(Unit& unit);
    virtual void _onEpisodeStart();
    virtual void _onEpisodeEnd();

    virtual void _onTileDeplete(Tile &);

};


#endif //WARC2SIM_GAME_H
