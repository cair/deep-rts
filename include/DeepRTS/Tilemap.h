//
// Created by Per-Arne on 24.02.2017.
//
#pragma once


#include "Constants.h"
#include "Tile.h"
#include "Map.h"
#include <vector>
namespace DeepRTS {

    class Game;

/// The tilemap class specifies the map of the game
/// This class is created from loading the map in the Map class
    class Tilemap {

    public:
        /// Reference to the game object
        Game &game;

        /// The constructor.
        /// \param map A reference to the map object
        /// \param game A reference to the game
        Tilemap(Map &map, Game &game);

        /// List of spawn tiles (Tiles that players can spawn on)
        std::vector<int> spawnTiles;

        /// Get all tiles in the tilemap
        /// \return
        std::vector<Tile> &getTiles();

        /// Get the corresponding spawn tile for a specific player ID (each player is locked to a spawn position)
        /// \param playerID
        /// \return Tile reference
        Tile &getSpawnTile(int playerID);

        /// Get a list of tiles that are neighbour to the input tile
        /// \param tile The input tile reference
        /// \param type Which type of tiles you wish to retrieve
        /// \return List of neighbors tiles
        std::vector<Tile *> neighbors(Tile &tile, Constants::Pathfinding type);

        /// Get tiles in an area around a tile
        /// \param source The source tile reference
        /// \param width Width of the area
        /// \param height Height of the area
        /// \return List of Tile pointers of the surrounding area of the source tile
        std::vector<Tile *> getTileArea(Tile &source, uint32_t width, uint32_t height);

        /// Get a tile on position X and Y
        /// \param x integer X position
        /// \param y integer Y position
        /// \return Tile reference
        Tile &getTile(uint32_t x, uint32_t y);

        /// List of all tiles in the tilemap
        std::vector<Tile> tiles;

        /// Reset the tilemap
        void reset();

        /// The implicit convertion is used in the JPS pathfinding. The call avaluates if the path is viable in a trajectory.
        /// \param x
        /// \param y
        /// \return
        bool operator()(unsigned x, unsigned y) const;
    };

}