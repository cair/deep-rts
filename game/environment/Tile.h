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

class Tile: public sf::Drawable{
public:
    Tile(int x, int y, int w, int h);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{};
    int id;
    std::string name;
    bool harvestable;
    bool walkable;
    bool swimable;
    int oilYield;
    int lumberYield;
    int goldYield;
    int resources;
    sf::Vertex vertices[4];

    Unit* occupant = NULL;

    bool isAttackable(Unit &unit);
    bool isWalkable();
    bool isHarvestable();
    void setOccupant(Unit &unit);



    int x;
    int y;


    const sf::Vector2f getPixelPosition();

    int height;
    int width;
};


#endif //WARC2SIM_TILE_H
