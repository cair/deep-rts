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
class Tile: public sf::Drawable{
public:
    Tile(int x, int y, int w, int h, Tilemap &tilemap);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{};
    int id_;
    int tId;
    std::string name;
    bool harvestable;
    bool walkable;
    bool swimable;
    int oilYield;
    int lumberYield;
    int goldYield;
    int resources;
    sf::Vertex vertices[4];
	sf::Color color;
    Tilemap &tilemap;
    std::shared_ptr<Unit> occupant = NULL;

    bool isAttackable(std::shared_ptr<Unit> unit);
    bool isWalkable();
    bool isHarvestable();
    void setOccupant(std::shared_ptr<Unit> unit);



    int x;
    int y;


    sf::Vector2f getPixelPosition()const;

    int height;
    int width;


    bool canWalkTo();

    int distance(Tile *pTile);

    bool isBuildable();
    int depleteTile;

    void setDepleted();
};


#endif //WARC2SIM_TILE_H
