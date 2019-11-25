//
// Created by Per-Arne on 24.02.2017.
//

#pragma once

#include <vector>
#include <memory>
#include <map>
#include <set>
#include <deque>
#include "../Constants.h"
#include "../environment/Tile.h"
#include "../state/StateManager.h"
#include "../util/Position.h"
#include "../Config.h"
#include "../Constants.h"

class Game;
class Player;
class BaseState;



class Unit{
private:
	std::deque<std::pair<Constants::Action, int>> actionQueue;


public:
	Game *game;

	const Config & config;

	/// Constructor
	explicit Unit(Player &player);


	//////////////////////////////////////////////////////////////
	///
	/// Properties
	///
	//////////////////////////////////////////////////////////////

	/// Unit ID
	int id;

	/// Unit Name ID (i.e Peasant1)
	std::string nameID;

	/// Unit Type
	Constants::Unit typeId;

	/// Unit Current Health
    int health;

	/// Unit Max Health
    int health_max;

	/// Unit Direction
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


    Player &player_;


    // State attributes

	// Spawning
	int spawnDuration;
	int spawnTileID = -1;
	int builtByID = -1;
	Unit &getBuiltBy();
	Tile &getSpawnTile();


    // Harvesting
    double harvestInterval = .5; // Must be set in constructor
    int harvestTimer = 0;
    int harvestIterator;
    int harvestTargetID = -1;
	Tile *getTile(int tileID);
    std::set<int> getVisionTileIDs();

    // Building
    int spawnTimer = 0;
    int buildEntityID = -1;
	Unit &getBuildEntity();
    int buildTimer = 0;

    // Combat
    int combatTargetID = -1;
	Unit *getCombatTarget();
    double combatTimer = 0;
    double combatInterval = 1; // Must be set in constructor

    // Walking
    int walking_timer = 0;
    std::vector<Tile *> walking_path;
    int walkingGoalID = -1;
	int stepsLeft = 0; // If value is 0 it is ignored. However, Walking is transitioned when stepsLeft = 1 (Which decrements it to 0)
    double walking_interval = 1; // Must be set in constructor

    // Animation
    int animationCounter = 0;


    Tile *tile = nullptr;
    StateManager* stateManager;
	std::shared_ptr<BaseState> state = nullptr;

    std::vector<Constants::State> stateList;


	Tile *getNextTile();
    bool build(int idx);
    void spawn(Tile &x, int initValue);
    void despawn();
    void update();
	Tile * centerTile();
    void enqueueState(const std::shared_ptr<BaseState>& state);
    void transitionState();
    void transitionState(std::shared_ptr<BaseState> nextState);
    void setPosition(Tile &tile);
    void rightClick(Tile &tile);
    void move(Tile &targetTile);
    void attack(Tile &tile);
    void harvest(Tile &tile);
    int distance(Tile &tile);
    int distance(Unit & unit);
	Position distanceVector(Tile &target);


    void clearTiles();

    Unit  *closestRecallBuilding();


    bool isDead();

    void afflictDamage(int dmg_);

    int getDamage(Unit &target);

    Position worldPosition;

    void setDirection(int newX, int newY);
    void setDirection(Position &dir);

    void moveRelative(int x, int y);

    void rightClickRelative(int x, int y);

    bool removedFromGame = false;

	bool operator==(int id) const;

	void tryMove(int16_t x, int16_t y);
	void tryAttack();
	void tryHarvest();

	Player &getPlayer();


    bool position_in_bounds(int x, int y);


	/// State alterations
	void setStateForTile(Tile * t);
	void clearStateForTile(Tile *t);
};

