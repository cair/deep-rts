//
// Created by Per-Arne on 23.02.2017.
//

#ifndef WARC2SIM_GAME_H
#define WARC2SIM_GAME_H


#include "defs.h"
#include <memory>
#include <map>
#include <unordered_map>
#include <iostream>
#include <chrono>
#include "Player.h"
#include "Map.h"
#include "Tilemap.h"
#include "Config.h"
#include <xtensor/xarray.hpp>
#include <cstdint>
#include <opencv2/core/mat.hpp>
namespace DeepRTS {

    class BaseGUI;

    using StateType = xt::xarray<double, xt::layout_type::row_major>;

    class Game {

        /// Game Clock
        std::chrono::high_resolution_clock::time_point now;
        std::chrono::high_resolution_clock::time_point _update_next;
        std::chrono::high_resolution_clock::time_point _stats_next;

        std::chrono::nanoseconds _update_interval{};

        uint32_t _update_delta = 0;

        /// Initialize the game clock timers
        void timerInit();

        ///
        bool running{};


        /// Inits neccerary memory in stack vectors
        void _internalInit();

    public:
        /// GUI Pointer
        std::unique_ptr<BaseGUI> gui;

        /// Game Constructor
        explicit Game(const std::string &map_file);

        Game(const std::string &map_file, Config config);

        ~Game();


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
        StateType state;

        /// Game Tilemap (Order 3)
        Tilemap tilemap;

        /// Game State Manager
        StateManager stateManager;

        /// List of Players inside the game session
        std::vector<Player> players;

        /// List of Units inside the game session
        std::vector<Unit> units;

        std::unordered_map<std::string, Unit *> unitsNameMap;

        /// Game Identification
        uint32_t id{};

        /// Game Episode Ticks
        uint64_t ticks = 0;

        /// Game Episode
        uint32_t episode = 1;

        /// Game Max FPS
        uint32_t max_fps{};


        /// Game Current FPS
        uint32_t currentFPS = 0;


        /// Game terminal flag
        bool terminal = false;

        /// Selected player
        Player *selectedPlayer{};

        ////////////////////////////////////////////////////
        ///
        /// Getters
        ///
        ////////////////////////////////////////////////////

        /// Get a Unit via index
        Unit &getUnit(int idx);

        uint32_t getMaxFPS() const;


        uint32_t getFPS() const;


        uint64_t getGameDuration() const;

        uint64_t getTicks() const;

        uint32_t getWidth() const;

        uint32_t getHeight() const;

        uint32_t getEpisode() const;

        uint32_t getId() const;

        long getTicksModifier() const;

        Unit *getUnitByNameID(const std::string &nameID);

        ////////////////////////////////////////////////////
        ///
        /// Setters
        ///
        ////////////////////////////////////////////////////

        /// Set the Game FPS
        void setMaxFPS(int fps_);

        /// Set selected player
        void setSelectedPlayer(Player &player);

        ////////////////////////////////////////////////////
        ///
        /// Engine Functions
        ///
        ////////////////////////////////////////////////////
        // Add a player to the game
        Player &addPlayer();

        // Insert a player using a precreated object. specifically useful for python
        void insertPlayer(Player &player);

        /// Game Tick Function
        void tick();

        /// Game Update Function
        virtual void update();

        virtual void _update();

        /// Game Render Function
        const cv::Mat &render();

        /// Game Render implementation
        virtual const cv::Mat &_render() const;

        /// Print the Game Statistics
        void caption();

        virtual void _caption() const;

        // Enables the running flag and updates the clock
        void start();

        // Disables the running flag
        void stop();

        // Resets the game
        virtual void reset();

        virtual void _reset();

        // Returns true if the game is in an terminal state
        bool isTerminal();

        ////////////////////////////////////////////////////
        ///
        /// Callback Functions
        ///
        ////////////////////////////////////////////////////
        // Unit Events
        virtual void _onUnitCreate(const Unit &unit) const;

        virtual void _onUnitDestroy(const Unit &unit) const;

        // Resource Events
        virtual void _onResourceGather(const Tile &tile, const Unit &unit) const;

        virtual void _onResourceDepleted(const Tile &tile, const Unit &unit) const;

        virtual void _onEpisodeStart() const;

        virtual void _onEpisodeEnd() const;

        virtual void _onTileChange(const Tile &) const;

        virtual void _onUnitAttack(const Unit &unit) const;

        /// empty opencv container in case of no GUI
        const cv::Mat renderPlaceholder;


    };
}

#endif //WARC2SIM_GAME_H
