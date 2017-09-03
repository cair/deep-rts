//
// Created by Per-Arne on 23.02.2017.
//

#include "Game.h"
#include "graphics/GUI.h"
#include "unit/UnitManager.h"
#include <chrono>

std::unordered_map<int, Game*> Game::games;
Game::Game(uint8_t _nplayers, bool setup):
        doCaptionWindow(Config::getInstance().getCaptionWindow()),
		doCaptionConsole(Config::getInstance().getCaptionConsole()),
        doScoreLogging(Config::getInstance().getLoggingScoring()),
        gameLog(*this),	// Only used when doScoreLogging flag is set in config
		doDisplay(Config::getInstance().getDisplay()),
		map(Tilemap("contested-4v4.json", this))


{
    players.reserve(_nplayers);
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
	// Reset GUI
	//gui->reset();

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
		gameLog.serialize();
		gameLog.reset();
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


	do {
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
				gameLog.record();	// Moves to "next element"
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

	} while(this->running);

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
	players.push_back(Player(*this, players.size()));
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
		builder.build(0);
	}

}

Unit & Game::getUnit(uint16_t idx)
{
	assert((idx >= 0 && idx < (units.size())) && "getUnit(idx) failed. Index not in range!");
	return units[idx];
}



void Game::deactivateGUI() {
	doDisplay = false;
	doCaptionWindow = false;
	doCaptionConsole = false;
	doScoreLogging = false;
}

void Game::load(Game *other) {
    // Load game data
    ticks = other->ticks;
    id = other->id;

	// Load tile data
	for(int i = 0; i < other->map.tiles.size(); i++) {
		auto &myTile = map.tiles[i];
		auto &otherTile = other->map.tiles[i];

		myTile.depleted = otherTile.depleted;
		myTile.harvestable = otherTile.harvestable;
		myTile.swimable = otherTile.swimable; // TODO uneccecary ?
		myTile.walkable = otherTile.walkable;
        myTile.setOccupantID(otherTile.getOccupantID());
		myTile.setResources(otherTile.getResources());

		assert(myTile.id == otherTile.id);
		assert(myTile.tileID == otherTile.tileID);
	}


    // Load Units
    units.clear();
    for(int i = 0; i < other->units.size(); i++) {
        auto &otherUnit = other->units[i];
        auto &myPlayer = players[otherUnit.player_->id_];

        units.push_back(UnitManager::constructUnit(otherUnit.typeId, &myPlayer));
        auto &myUnit = units.back();

        myUnit.id = otherUnit.id;
        myUnit.builtByID = otherUnit.builtByID;
        myUnit.buildTimer = otherUnit.buildTimer;
        myUnit.buildEntityID = otherUnit.buildEntityID;
        myUnit.state = otherUnit.state;
        myUnit.spawnTileID = otherUnit.spawnTileID;
        myUnit.spawnTimer = otherUnit.spawnTimer;

        myUnit.animationInterval = otherUnit.animationInterval;
        myUnit.animationIterator = otherUnit.animationIterator;
        myUnit.animationTimer = otherUnit.animationTimer;

        if(otherUnit.tile){
            myUnit.tile = &map.tiles[otherUnit.tile->id];
        }else {
            myUnit.tile = NULL;
        }

        myUnit.walking_timer = otherUnit.walking_timer;
        myUnit.walkingGoalID = otherUnit.walkingGoalID;
        myUnit.walking_interval = otherUnit.walking_interval;
        std::vector<Tile *> newPath;
        for(auto &t :otherUnit.walking_path) {
            newPath.push_back(&map.tiles[t->id]);
        }
        myUnit.walking_path = newPath;

        myUnit.combatInterval = otherUnit.combatInterval;
        myUnit.combatTargetID = otherUnit.combatTargetID;
        myUnit.combatTimer = otherUnit.combatTimer;

        myUnit.harvestTargetID = otherUnit.harvestTargetID;
        myUnit.harvestInterval = otherUnit.harvestInterval;
        myUnit.harvestIterator = otherUnit.harvestIterator;
        myUnit.harvestTimer = otherUnit.harvestTimer;

    }


    // Load Players
    for(int i = 0; i < other->players.size(); i++) {
        auto &myPlayer = players[i];
        auto &otherPlayer = other->players[i];

        myPlayer.targetedUnitID = otherPlayer.targetedUnitID;
        myPlayer.defeated = otherPlayer.defeated;
        myPlayer.food = otherPlayer.food;
        myPlayer.foodConsumption = otherPlayer.foodConsumption;
        myPlayer.gold = otherPlayer.gold;
        myPlayer.lumber = otherPlayer.lumber;
        myPlayer.oil = otherPlayer.oil;
        myPlayer.statGoldGather = otherPlayer.statGoldGather;
        myPlayer.statLumberGather = otherPlayer.statLumberGather;
        myPlayer.statOilGather = otherPlayer.statOilGather;
        myPlayer.statUnitBuilt = otherPlayer.statUnitBuilt;
        myPlayer.statUnitDamageDone = otherPlayer.statUnitDamageDone;
        myPlayer.statUnitDamageTaken = otherPlayer.statUnitDamageTaken;
        myPlayer.statUnitMilitary = otherPlayer.statUnitMilitary;

        myPlayer.unitIndexes.clear();
        myPlayer.unitIndexes.insert(std::end(myPlayer.unitIndexes), std::begin(otherPlayer.unitIndexes), std::end(otherPlayer.unitIndexes));

        assert(myPlayer.id_ == otherPlayer.id_);
    }





}




