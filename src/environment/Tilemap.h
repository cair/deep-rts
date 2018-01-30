//
// Created by Per-Arne on 24.02.2017.
//
#pragma once


#include "../Constants.h"
#include "Tile.h"
#include <vector>
#include "../../include/rapidjson/document.h"

class Game;

class Tilemap{
private:
	Game &game_;


public:

	/// Constructor
	Tilemap(std::string mapName, Game &game);

	/// List of spawn tiles (Tiles that players can spawn on)
	std::vector<int> spawnTiles;

	/// A tile's Width
	int TILE_WIDTH;

	/// A tile's Height
	int TILE_HEIGHT;

	/// The Map Width (Num of tiles in width)
	int MAP_WIDTH;

	/// The Map Height (Num of tiles in height)
	int MAP_HEIGHT;

	/// Get all tiles
	std::vector<Tile>& getTiles();

	/// Get a list of tiles that are neighbour to the input tile
	std::vector<Tile *> neighbors(Tile &tile, Constants::Pathfinding type);

	/// Get tiles in an area around a tile
	std::vector<Tile *> getTileArea(Tile &source, int width, int height);

	/// Get a tile on position X and Y
	Tile &getTile(int x, int y);

	/// Get the game reference
	Game &getGame() const;

	/// List of tiles
	std::vector<Tile> tiles;



	bool operator()(unsigned x, unsigned y) const;
};
