//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_PLAYER_H
#define WARC2SIM_PLAYER_H

#include <string>
#include <vector>
#include <memory>
#include <deque>
#include <cmath>

#include "../unit/Unit.h"
#include "../environment/Tile.h"
#include "../Config.h"

class Game;
class Player {
private:

	/// Players game reference
	Game &game_;

private:
    void spawnPlayer();

	/// Player id
	int id_;

	/// Player name
    std::string name = "[NO NAME]";

	/// Get index of next or previous unit
	int _getNextPrevUnitIdx();

	/// Faction of the Player
	int faction; // 0 = Human, 1 = Orc

	///////////////////////////////////////////////////////////
	///
	/// Player State variables
	///
	///////////////////////////////////////////////////////////

	/// ID of targeted unit (-1 = None)
	int targetedUnitID = -1;

	/// Determines if the player is defeated or not
	bool defeated = false;

	/// Selects next unit in players units list
	void nextUnit();

	/// Selects previous unit in players units list
	void previousUnit();



public:
	const Config &config;

	/// List of all unit Ids belonging to this player. The object itself is located in Game class units list
	std::vector<int> unitIndexes;

	/// Constructor
	Player(Game &game, int id);


	////////////////////////////////////////////////////
	///
	/// Setters
	///
	////////////////////////////////////////////////////

	/// Set the name of the player
	void setName(std::string name);

	/// Select a unit
	void setTargetedUnitID(int targetedUnitID);

	////////////////////////////////////////////////////
	///
	/// Getters
	///
	////////////////////////////////////////////////////

	/// Get the game instance
	Game &getGame() const;

	/// Get Player's ID
	int getId();

	/// Get the food consumption
	int getFoodConsumption();

	/// Get Max Food
	int getFood();

	/// Get the gold resource
	int getGold();

	/// Get the oil resource
	int getOil();

	/// Get the lumber resource
	int getLumber();

	/// Get the score
 	int getScore();

	/// Get the unit count for this player
	int getUnitCount();

	/// Get the targeted unit (Returns null if none)
	Unit *getTargetedUnit();

	/// Get the targeted unit ID (-1 if none)
	int getTargetedUnitID() const;

	/// Check if player is defeated
	bool isDefeated();

	/// Get the player name
	const std::string &getName() const;


	////////////////////////////////////////////////////
	///
	/// Functions
	///
	////////////////////////////////////////////////////

	/// Add a unit to the player
	Unit &addUnit(Constants::Unit unitType);

	/// Removes a unit from the player
	void removeUnit(Unit & unit);

	/// Spawns a unit on a tile
	Unit &spawn(Tile &spawnPoint);

	/// Reset the player state
	void reset();

	/// Remove N gold from the player
	void removeGold(int n);

	/// Remove N lumber from the player
	void removeLumber(int n);

	/// Remove N oil from the player
	void removeOil(int n);

	/// Add N gold to the player
	void addGold(int n);

	/// Add N lumber to the player
	void addLumber(int n);

	/// Add N oil to the player
	void addOil(int n);

	///////////////////////////////////////////////////////////
	///
	/// Player Resources
	///
	///////////////////////////////////////////////////////////
	int oil = 0;
	int gold = 0;
	int lumber = 0;
	int foodConsumption = 0;
	int food = 0;

	/// Counters for buildings
	int num_archer = 0;
	int num_peasant = 0;
	int num_footman = 0;
	int num_town_hall = 0;
	int num_farm = 0;
	int num_barrack = 0;

	///////////////////////////////////////////////////////////
	///
	/// Player Statistics
	///
	///////////////////////////////////////////////////////////
	int sGatheredGold = 0;
	int sGatheredLumber = 0;
	int sGatheredOil = 0;
	int sDamageDone = 0;
	int sDamageTaken = 0;
	int sUnitsCreated = 0;


	void do_action(int actionID);
	void do_manual_action(int manual_action_id, int x, int y);

	void leftClick(int x, int y);
	void rightClick(int x, int y);

	/// TODO move these to Unit.h?
    bool canPlace(Unit & builder, Unit & unit, Tile &tile);
    bool canAfford(Unit & unit);















};


#endif //WARC2SIM_PLAYER_H
