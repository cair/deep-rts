//
// Created by Per-Arne on 23.02.2017.
//

#include "Game.h"


#include "graphics/GUI.h"
#include "graphics/Animation.h"
#include "Config.h"
#include "third_party\json.hpp"

std::unordered_map<int, Game*> Game::games;

Game::Game(uint8_t _nplayers, bool setup):
        map(Tilemap("contested-4v4.json")),
		captionConsole(Config::getInstance().getCaptionConsole()),
		captionWindow(Config::getInstance().getCaptionWindow())
{
	players.reserve(16);
	units.reserve(1000);

    // Definitions
    n_players = _nplayers;
    setFPS(Config::getInstance().getFPS());
    setUPS(Config::getInstance().getUPS());

	id = static_cast<uint8_t>(games.size());
	games[id] = this;


    if(setup)
        Animation::getInstance().setup();


}

void Game::createPlayers(){

    for (uint8_t i = 0; i < n_players; i++) {
        addPlayer();

    }

}

void Game::initGUI(){
    this->gui = new GUI(*this);
}


void Game::setFPS(uint32_t fps_){
    fps = fps_;
    _render_interval = static_cast<clock_t>(1000.0 / fps);


}

void Game::setUPS(uint32_t ups_){
    ups = ups_;
    _update_interval =  static_cast<clock_t>(1000.0 / ups);
}

void Game::start(){
    this->running = true;
}

void Game::stop(){
    this->running = false;
}

void Game::reset()
{
	units.clear();
	for (auto &tile : map.tiles) {
		tile.reset();
	}
	for (auto &player : players) {
		player.reset();
		spawnPlayer(player);
	}
	ticks = 0;


	if (Config::getInstance().getJsonLogging()) {
		std::string input = jsonStatContainer.dump();
		std::ofstream out("games/deeprts_game_" + std::to_string(gameNum) + ".json");
		out << input;
		out.close();
		jsonStatContainer.clear();
	}

	gameNum += 1;

	

}


void Game::loop() {

    clock_t now = clock();
    this->_render_next= now + this->_render_interval;
    this->_update_next = now + this->_update_interval;
    this->_stats_next = now + 0;


    while(this->running) {


        now = clock();

        if (now >= _update_next) {
            // Update

            for(auto &unit : units) {
				if (unit.removedFromGame) continue;		// Skip unit that is removed from game
				unit.update();
            }

			for (auto &p : players) {
				p.update();
				if(p.algorithm_)
					p.algorithm_->update();
			}

			// Output all scores etc to file for each game
			if (Config::getInstance().getJsonLogging()) {
				// Record data
				for (auto &p : players) {

					json stat;
					for (int i = 0; i < sizeof(Constants::actionNames) / sizeof(Constants::actionNames[0]); i++) {
						stat.push_back(p.actionStatistics[i]);
					}

					json player;
					player["action_stats"] = stat;
					player["apm"] = p.apm;
					player["score"] = p.getScore();
					player["tick"] = ticks;
					jsonStatContainer[p.id_].push_back(player);
				}
			}

            _update_next += _update_interval;
            _update_delta += 1;
            ticks += 1;

        }

        if (now >= _render_next) {
            // Render

            gui->update();
            gui->render();


            this->_render_next += this->_render_interval;
            this->_render_delta += 1;
        }

        if (now >= this->_stats_next) {

			if (captionWindow) {
				gui->caption();
			}
           
			if (captionConsole) {
				std::cout << "[FPS=" << this->currentFPS << ", UPS=" << this->currentUPS << "]" << std::endl;
			}

			// Calculate APM
			if (true /*calculateAPM*/) {
				for (auto &p : players) {
					p.apm = (p.apm + (p.apm_counter * 60)) / 2; // Multiplies by 60 since sampling is per second
					p.apm_counter = 0;
				}
			}
           

            this->currentFPS = this->_render_delta;
            this->currentUPS = this->_update_delta;
            this->_render_delta = 0;
            this->_update_delta = 0;
            this->_stats_next += 1000;
        }

    }

}

Tilemap &Game::getMap() {
	return map;
}

uint64_t Game::getFrames() {
    return this->ticks;
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

	// If json config logging is enabled
	if (Config::getInstance().getJsonLogging()) {
		jsonStatContainer[player.id_] = {};
	}

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



