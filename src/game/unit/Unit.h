//
// Created by Per-Arne on 24.02.2017.
//

#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include <map>
#include <SFML/Graphics/Texture.hpp>
#include "../Constants.h"

#include "../environment/Tile.h"
#include "../state/StateManager.h"
#include "../Config.h"


class Player;
class BaseState;



class Unit{
private:
    static int gId;
public:
	Unit(Player *player);
	Unit() {}

    // Graphics
    double animationInterval = .5 * 10;
    int animationTimer = 0;
    int animationIterator = 0;

    int id;
	Constants::Unit typeId;
    int health;
    int health_max;
    int current_state;
    int direction;


    int groundUnit;
    int waterUnit;

    int damageMin = -1;
    int damageMax = -1;
    int damageRange = -1;
    int damagePiercing = -1;
    int armor = -1;

    int lumberCarry = 0;
    int goldCarry = 0;
    int oilCarry = 0;
    int carryCapacity = 10;
    std::vector<Constants::Unit> buildInventory;
    int speed;
    int sight;
    int range = 4;

    bool canHarvest;
    bool canAttack;
    bool canMove;
    bool military;
    bool structure;
    bool recallable = false;

    int lumberCost;
    int goldCost;
    int oilCost;

    int foodProduction;
    int foodConsumption;

    int width;
    int height;

    std::string name;


    Player *player_;


    // State attributes

	// Spawning
	int spawnDuration;
	Tile *spawnTile = NULL;
	Unit* builtBy = NULL;


    // Harvesting
    double harvestInterval = .5 * 10;
    int harvestTimer;
    int harvestIterator;
    Tile *harvestTarget = NULL;

    // Building
    int spawnTimer;
    Unit* buildEntity = NULL;
    int buildTimer;

    // Combat
    Unit* combatTarget = NULL;
    int combatTimer = 1000;
    double combatInterval = 1 * 10;

    // Walking
    int walking_timer = 0;
    std::vector<Tile *> walking_path;
    Tile *walkingGoal = NULL;
    double walking_interval = .1 * 10;


    Tile *tile = NULL;
    StateManager* stateManager;
	std::shared_ptr<BaseState> state = NULL;

    std::vector<std::shared_ptr<BaseState>> stateList;



    bool build(int idx);
    void spawn(Tile &x, int initValue);
    void despawn();
    void update();
	Tile * centerTile();
    void enqueueState(std::shared_ptr<BaseState> state);
    void transitionState();
    void transitionState(std::shared_ptr<BaseState> nextState);
    void setPosition(Tile &tile);
    void rightClick(Tile &tile);
    void move(Tile &targetTile);
    void attack(Tile &tile);
    void harvest(Tile &tile);
    int distance(Tile &tile);
    int distance(Unit & unit);
    sf::Vector2f distanceVector(Tile &target);


    void clearTiles();

    Unit  *closestRecallBuilding();


    bool isDead();

    void afflictDamage(int dmg_);

    int getDamage(Unit &target);

    sf::Vector2f worldPosition;

    void setDirection(int newX, int newY);
    void setDirection(sf::Vector2f &dir);

    void moveRelative(int x, int y);

    void rightClickRelative(int x, int y);

    bool removedFromGame = false;

	bool operator==(int id) const;
	

};
