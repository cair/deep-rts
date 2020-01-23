//
// Created by Per-Arne on 24.02.2017.
//
#pragma once


#include "../Constants.h"
#include "Tile.h"
#include "Map.h"
#include <vector>
#include "../../../include/rapidjson/include/rapidjson/document.h"

class Game;

class Tilemap{

public:
	Game &game;

	/// Constructor
	Tilemap(Map& map, Game &game);

	/// List of spawn tiles (Tiles that players can spawn on)
	std::vector<int> spawnTiles;

	/// Get all tiles
	std::vector<Tile>& getTiles();

	/// Get a list of tiles that are neighbour to the input tile
	std::vector<Tile *> neighbors(Tile &tile, Constants::Pathfinding type);

	/// Get tiles in an area around a tile
	std::vector<Tile *> getTileArea(Tile &source, int width, int height);

	/// Get a tile on position X and Y
	Tile &getTile(int x, int y);

	/// List of tiles
	std::vector<Tile> tiles;

	/// Reset the tilemap
	void reset();



	bool operator()(unsigned x, unsigned y) const;
};
