//
// Created by Per-Arne on 23.02.2017.
//

#include "Game.h"


#include "graphics/GUI.h"
#include "Config.h"
#include "third_party/json.hpp"

std::unordered_map<int, Game*> Game::games;

Game::Game(uint8_t _nplayers, bool setup):
		map(Tilemap("contested-4v4.json")),
		doCaptionConsole(Config::getInstance().getCaptionConsole()),
		doCaptionWindow(Config::getInstance().getCaptionWindow()),
		doDisplay(Config::getInstance().getDisplay()),
		doScoreLogging(Config::getInstance().getLoggingScoring()),
		scoreLog(_nplayers)	// Only used when scoreLog flag is set in config
{
	players.reserve(16);
	units.reserve(1000);


    // Definitions
    n_players = _nplayers;
    setFPS(Config::getInstance().getFPS());
    setUPS(Config::getInstance().getUPS());
	setAPM(Config::getInstance().getAPM());
	tickReset = Config::getInstance().getTickReset();



	id = static_cast<uint8_t>(games.size());
	games[id] = this;

}

void Game::createPlayers(){

	for (uint8_t i = 0; i < n_players; i++) {
		addPlayer();

	}

}

void Game::initGUI(){
	if(doDisplay)
		this->gui = new GUI(*this);
}


void Game::setFPS(uint32_t fps_){
	fps = fps_;
	_render_interval = 1000000 /  fps;


}

void Game::setUPS(uint32_t ups_){
	ups = ups_;
	_update_interval = 1000000 / ups;
}

void Game::setAPM(uint16_t apm_)
{
	apm = apm_;
	_apm_interval = 1000000 / (apm / 60.0);
}

void Game::triggerResetNow()
{
	triggerReset = true;
}

void Game::start(){
	this->running = true;
}

void Game::stop(){
	this->running = false;
}

void Game::reset()
{
	// Remove all units
	units.clear();

	// Reset all tiles
	for (auto &tile : map.tiles) {
		tile.reset();
	}

	// Reset all players
	for (auto &player : players) {
		player.reset();
		spawnPlayer(player);
	}

	// Reset tick counter
	ticks = 0;

	// Sav
	if (doScoreLogging) {
		scoreLog.serialize(gameNum, "games/deeprts_game_" + std::to_string(gameNum) + ".flat");
		scoreLog.reset();
	}
	gameNum += 1;



}


void Game::loop() {



	sf::Time now = clock.getElapsedTime();
	auto nowMicroSec = now.asMicroseconds();
    _render_next = nowMicroSec + _render_interval;
    _update_next = nowMicroSec + _update_interval;
	_apm_next = nowMicroSec + _apm_interval;
    _stats_next = nowMicroSec + 0;


	while(this->running) {
		now = clock.getElapsedTime();		// Update clock
		nowMicroSec = now.asMicroseconds();
		if (nowMicroSec >= _update_next) {
			// If reset flag is set
			if (ticks > tickReset || triggerReset) {
				reset();
				triggerReset = false;

				// Update Counters and statistics
				_update_next += _update_interval;
				_update_delta += 1;
				ticks += 1;

				continue;
			}



			// Iterate through all units
			for(auto &unit : units) {
				if (unit.removedFromGame) continue;		// Skip unit that is removed from game
				unit.update();
			}

			// Iterate through all players
			for (auto &p : players) {
				p.update();
			}

			///
			/// Algorithm Update
			///
			if (nowMicroSec >= _apm_next) {
				for (auto &p : players) {
					if (!p.algorithm_) continue;

					p.algorithm_->update();
				}

				_apm_next += _apm_interval;
			}

			
				
					



			// Output all scores etc to file for each game
			if (doScoreLogging) {

				int i = 0;
				for (auto &p : players) {
					scoreLog.addElement(i++, p); // Add element for player
				}
				scoreLog.nextTick(ticks);	// Moves to "next element"
			}

			// Update Counters and statistics
			_update_next += _update_interval;
			_update_delta += 1;
			ticks += 1;

		}

		if (doDisplay && nowMicroSec >= _render_next) {
			// Render

			gui->update();
			gui->render();


			this->_render_next += this->_render_interval;
			this->_render_delta += 1;
		}

		if (nowMicroSec >= this->_stats_next) {

			if (doDisplay && doCaptionWindow) {
				//gui->caption();
			}

			if (doCaptionConsole) {
				std::cout << "[FPS=" << this->currentFPS << ", UPS=" << this->currentUPS << "]" << std::endl;
			}

			// Calculate APM
			if (true /*calculateAPM*/) {
				for (auto &p : players) {
					p.apm = (p.apm + (p.apm_counter * 60)) / 2; // Multiplies by 60 since sampling is per second
					p.apm_counter = 0;
				}
			}


			currentFPS = _render_delta;
			currentUPS = _update_delta;
			_render_delta = 0;
			_update_delta = 0;
			_stats_next += 1000000;
		}

	}

}

Tilemap &Game::getMap() {
	return map;
}

uint64_t Game::getFrames() {
	return this->ticks;
}

uint32_t Game::getGameCount()
{
	return gameNum;
}

Game * Game::getGame(uint8_t id)
{
	Game *g = games.at(id);
	assert(g);
	return g;
}

uint64_t Game::getSeconds() {
	return this->ticks / Config::getInstance().getTickModifier();
}

bool Game::checkTerminal(){

	int c = 0;
	for(auto &p : players) {
		if(p.defeated){
			c++;
		}
	}


	bool isTerminal = (c == 1);
	terminal = isTerminal;
	if (terminal) {
		setUPS(0);
	}

	return terminal;
}

void Game::addAction(std::shared_ptr<BaseAction> action) {
	executedActions.push_back(action);
}

Player &Game::addPlayer() {
	players.push_back(Player(*this));
	Player &player = players.back();

	spawnPlayer(player);
	return player;
}

void Game::spawnPlayer(Player &player) {
	// Retrieve spawn_point
	int spawnPointIdx = map.spawnTiles[player.id_];

	// Spawn Initial builder
	Unit &builder = player.spawn(map.tiles[spawnPointIdx]);

	// If auto-spawn town hall mechanic is activated
	if (Config::getInstance().getMechanicTownHall()) {
		// build Town-Hall
		//builder.build(0);
	}

}

Unit & Game::getUnit(uint16_t idx)
{
	assert((idx >= 0 && idx < (units.size())) && "getUnit(idx) failed. Index not in range!");
	return units[idx];
}



std::string Game::serialize_json() {
	json data;

	/*data["tileIds"] .push_back();
	data["tileResources"] .push_back();
	data["tileOccupant"] .push_back();
	data["unitsDirection"] .push_back();
	data["unitsHarvestTimer"] .push_back();
	data["unitsHarvestIterator"] .push_back();
	data["unitsLumberCarry"] .push_back();
	data["unitsOilCarry"] .push_back();
	data["unitsGoldCarry"] .push_back();
	data["unitsSpawnTimer"] .push_back();
	data["unitsBuildTimer"] .push_back();
	data["unitsCombatTimer"] .push_back();
	data["unitsWalkingTimer"] .push_back();
	data["unitsType"] .push_back();
	data["unitsHealth"] .push_back();
	data["unitsIds"] .push_back();
	data["unitsTileID"] .push_back();
	data["unitsState"] .push_back();
	data["unitsCombatTarget"] .push_back();
	data["unitsBuildEntity"] .push_back();
	data["unitsWalkingGoal"] .push_back();
	data["unitsHarvestTarget"] .push_back();
	data["unitsSpawnTile"] .push_back();
	data["unitsPlayerID"] = {}
	data["playerID"].push_back(pid);
	data["defeated"].push_back(defeated);
	data["statGoldGather"].push_back(statGoldGather);
	data["statLumberGather"].push_back(statLumberGather);
	data["statOilGather"].push_back(statOilGather);
	data["statUnitDamageTaken"].push_back(statUnitDamageTaken);
	data["statUnitDamageDone"].push_back(statUnitDamageDone);
	data["statUnitBuilt"].push_back(statUnitBuilt);
	data["statUnitMilitary"].push_back(statUnitMilitary);

	data["food"].push_back(food);
	data["consumedFood"].push_back(consumedFood);
	data["gold"].push_back(gold);
	data["lumber"].push_back(lumber);
	data["oil"].push_back(oil);
	data["unitCount"].push_back(unitCount);
	data["gameTicks"] = gameTicks;*/

	///
	/// TILE DATA
	///
	const uint16_t numTiles = static_cast<uint16_t>(map.tiles.size());


	for (int i = 0; i < numTiles; i++) {
		Tile &t = map.tiles[i];
		data["tileIds"].push_back(t.id);
		data["tileResources"].push_back(t.getResources());
		data["tileOccupant"].push_back((t.getOccupant()) ? t.getOccupant()->id : -1);
	}



	///
	/// PLAYER DATA
	///
	size_t numUnits = 0;
	for (auto &p : players) {
		numUnits += p.unitIndexes.size();
	}

	int i = 0;
	for (auto &u : units) {
		//std::vector<BaseState *> stateList;
		//std::vector<Tile *> walking_path;

		data["unitsDirection"].push_back(u.direction);
		data["unitsHarvestTimer"] .push_back(u.harvestTimer);
		data["unitsHarvestIterator"] .push_back(u.harvestIterator);
		data["unitsLumberCarry"] .push_back(u.lumberCarry);
		data["unitsOilCarry"] .push_back(u.oilCarry);
		data["unitsGoldCarry"] .push_back(u.goldCarry);
		data["unitsSpawnTimer"] .push_back(u.spawnTimer);
		data["unitsBuildTimer"] .push_back(u.buildTimer);
		data["unitsCombatTimer"] .push_back(u.combatTimer);
		data["unitsWalkingTimer"] .push_back(u.walking_timer);
		data["unitsType"] .push_back(u.typeId);
		data["unitsHealth"] .push_back(u.health);
		data["unitsIds"] .push_back(u.id);
		data["unitsTileID"] .push_back((u.tile) ? u.tile->id : -1);
		data["unitsState"] .push_back(u.state->id);
		data["unitsPlayerID"].push_back(u.player_->id_);

		if (u.combatTarget)
			data["unitsCombatTarget"].push_back(u.combatTarget->id);
		else
			data["unitsCombatTarget"].push_back(-1);

		if (u.buildEntity)
			data["unitsBuildEntity"].push_back(u.buildEntity->id);
		else
			data["unitsBuildEntity"].push_back(-1);

		if (u.walkingGoal)
			data["unitsWalkingGoal"].push_back(u.walkingGoal->id);
		else
			data["unitsWalkingGoal"].push_back(-1);

		if (u.harvestTarget)
			data["unitsHarvestTarget"].push_back(u.harvestTarget->id);
		else
			data["unitsHarvestTarget"].push_back(-1);

		if (u.spawnTile)
			data["unitsSpawnTile"].push_back(u.spawnTile->id);
		else
			data["unitsSpawnTile"].push_back(-1);




		i++;
	}

	for(auto &p : players){


		int pid = p.id_;
		int defeated = p.defeated;
		int statGoldGather = p.statGoldGather;
		int statLumberGather = p.statLumberGather;
		int statOilGather = p.statOilGather;
		int statUnitDamageTaken = p.statUnitDamageTaken;
		int statUnitDamageDone = p.statUnitDamageDone;
		int statUnitBuilt = p.statUnitBuilt;
		int statUnitMilitary = p.statUnitMilitary;

		int food = p.getFood();
		int consumedFood = p.getFoodConsumption();
		int gold = p.getGold();
		int lumber = p.getLumber();
		int oil = p.getOil();
		int unitCount = p.getUnitCount();

		data["playerID"].push_back(pid);
		data["defeated"].push_back(defeated);
		data["statGoldGather"].push_back(statGoldGather);
		data["statLumberGather"].push_back(statLumberGather);
		data["statOilGather"].push_back(statOilGather);
		data["statUnitDamageTaken"].push_back(statUnitDamageTaken);
		data["statUnitDamageDone"].push_back(statUnitDamageDone);
		data["statUnitBuilt"].push_back(statUnitBuilt);
		data["statUnitMilitary"].push_back(statUnitMilitary);

		data["food"].push_back(food);
		data["consumedFood"].push_back(consumedFood);
		data["gold"].push_back(gold);
		data["lumber"].push_back(lumber);
		data["oil"].push_back(oil);
		data["unitCount"].push_back(unitCount);

	}

	///
	/// Game Stuff
	///
	int gameTicks = ticks;
	data["gameTicks"] = gameTicks;
	data["mapSize"] = { map.MAP_WIDTH, map.MAP_HEIGHT };


	return data.dump();


}

void Game::deactivateGUI() {

}




