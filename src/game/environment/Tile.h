//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_TILE_H
#define WARC2SIM_TILE_H

#include <string>
#include "../environment/Tile.h"

#include <SFML/Graphics/Sprite.hpp>
#include <memory>

class Unit;

class Tilemap;
class Tile{
private:
	// Tile data
	uint16_t resources;
	Unit* occupant = NULL;

public:
    Tile(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Tilemap &tilemap);

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

	// GUI
	bool needRedraw = true;
	Tilemap &tilemap;
    sf::Vertex vertices[4];

	// Check functions
    bool isAttackable(Unit & unit);
    bool isWalkable()const;
    bool isHarvestable()const;
	bool canWalkTo()const;
	bool isBuildable()const;
	
	// Info Functions
	uint16_t distance(Tile *pTile);

	// Setters
	void setOccupant(Unit *unit);
	void setDepleted();
	void setResources(uint16_t resource_count);
	void takeResource(uint8_t n);

	// Getters
	sf::Vector2f getPixelPosition()const;
	Unit* getOccupant();
	uint16_t getResources();











    

   
    

};


#endif //WARC2SIM_TILE_H
