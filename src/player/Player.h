//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_PLAYER_H
#define WARC2SIM_PLAYER_H

#include <string>
#include <vector>
#include <memory>
#include "../unit/Unit.h"
#include "../environment/Tile.h"
#include <deque>

class Game;
class Player {
private:

	/// Players game reference
	Game &game_;

private:

	/// Player id
	int id_;

	/// Player name
    std::string name = "[NO NAME]";

	/// Action queue
	std::deque<std::pair<Constants::Action, int>> actionQueue;

	/// Unit color definition
	std::tuple<uint8_t , uint8_t, uint8_t> playerColor;

	/// List of all unit Ids belonging to this player. The object itself is located in Game class units list
	std::vector<uint16_t> unitIndexes;

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
	bool defeated;

	/// Selects next unit in players units list
	void nextUnit();

	/// Selects previous unit in players units list
	void previousUnit();



public:

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
	size_t getUnitCount();

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

	/// Update the player state
    void update();

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
	int oil;
	int gold;
	int lumber;
	int foodConsumption;
	int food;

	///////////////////////////////////////////////////////////
	///
	/// Player Statistics
	///
	///////////////////////////////////////////////////////////
	int sGatheredGold;
	int sGatheredLumber;
	int sGatheredOil;
	int sDamageDone;
	int sDamageTaken;
	int sUnitsCreated;





	void queueAction(Constants::Action actionI, int delay);
	void queueActionA(int actionID, int delay);
	int actionStatistics[20];
	size_t getQueueSize();










 
	/// TODO move these to Unit.h?
    bool canPlace(Unit & builder, Unit & unit, Tile &tile);
    bool canAfford(Unit & unit);














    void rightClick(Position pos);
};


#endif //WARC2SIM_PLAYER_H
