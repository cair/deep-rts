//
// Created by Per-Arne on 24.02.2017.
//

#pragma once

#include <string>
#include "../environment/Tile.h"
#include "../util/Position.h"
#include <stdint.h>

class Unit;

class Tilemap;
class Tile{
private:
	// Tile data
	uint16_t resources;
	int occupantID = -1;

	const bool originalWalkable;
	const bool originalHarvestable;
	const uint16_t originalResources;
public:
    Tile(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Tilemap &tilemap, bool originalWalkable, bool originalHarvestable, uint16_t originalResources);

    // Tile data
    std::string name;
	uint16_t id;
	uint16_t tileID;
	uint8_t oilYield;
	uint8_t lumberYield;
	uint8_t goldYield;
	uint16_t depleteTile;
	bool harvestable;
	bool walkable;
    bool swimable;


	// Tile Properties
	uint16_t x;
	uint16_t y;
	uint8_t height;
	uint8_t width;

	Tilemap &tilemap;

	Position getPosition();


	// Check functions
    bool isAttackable(Unit & unit);
    bool isWalkable()const;
    bool isHarvestable()const;
	bool isBuildable()const;
	
	// Info Functions
	uint16_t distance(Tile *pTile);

	// Setters
	void setOccupant(Unit *unit);
	void setOccupantID(int unitID);
	void setDepleted();
	void setResources(uint16_t resource_count);
	void takeResource(uint8_t n);

	// Getters
	Unit* getOccupant();
	int getOccupantID();
	uint16_t getResources();


	void reset();


	bool depleted = false;

    bool hasOccupant();
};



