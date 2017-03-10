//
// Created by Per-Arne on 23.02.2017.
//

#include "Game.h"


#include "graphics/GUI.h"
#include "graphics/Animation.h"
#include "proto/GameMessage.pb.h"

std::unordered_map<int, Game*> Game::games;

Game::Game(int _nplayers, bool setup):
        map(Tilemap("contested-4v4.json"))
{
    // Definitions
    n_players = _nplayers;
    setFPS(Config::getInstance().getFPS());
    setUPS(Config::getInstance().getUPS());

	id = games.size();
	games[id] = this;


    if(setup)
        Animation::getInstance().setup();


}

void Game::createPlayers(){

    for (int i = 0; i < n_players; i++) {
        addPlayer();

    }

}

void Game::initGUI(){
    this->gui = new GUI(*this);
}

void Game::load_players(){

}

void Game::setFPS(int fps_){
    fps = fps_;
    _render_interval = 1000.0 / fps;


}

void Game::setUPS(int ups_){
    ups = ups_;
    _update_interval =  1000.0 / ups;
}

void Game::start(){
    this->running = true;
}

void Game::stop(){
    this->running = false;
}


void Game::loop() {

    clock_t now = clock();
    this->_render_next= now + this->_render_interval;
    this->_update_next = now + this->_update_interval;
    this->_stats_next = now + 0;


    while(this->running) {


        now = clock();

        if (now >= this->_update_next) {
            // Update

            for(auto p : this->players) {
                p->update();
            }

            this->_update_next += this->_update_interval;
            this->_update_delta += 1;
            this->ticks += 1;

        }

        if (now >= this->_render_next) {
            // Render

            gui->update();
            gui->render();


            this->_render_next += this->_render_interval;
            this->_render_delta += 1;
        }

        if (now >= this->_stats_next) {

            gui->caption();
            std::cout << "[FPS=" << this->currentFPS << ", UPS=" << this->currentUPS<< "]" << std::endl;

            this->currentFPS = this->_render_delta;
            this->currentUPS = this->_update_delta;
            this->_render_delta = 0;
            this->_update_delta = 0;
            this->_stats_next += 1000;
        }

    }



}

long Game::getFrames() {
    return this->ticks;
}

Game * Game::getGame(int id)
{
	Game *g = games.at(id);
	assert(g);
	return g;
}

int Game::getSeconds() {
    return this->ticks / Config::getInstance().getTickModifier();
}

bool Game::checkTerminal(){

    int c = 0;
    for(auto &p : players) {
        if(p->defeated){
            c++;
        }
    }

	std::cout << c << std::endl;
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
    std::shared_ptr<Player> player = std::shared_ptr<Player>(new Player(*this));


    // Retrieve spawn_point
    int spawnPointIdx = map.spawnTiles[players.size()];
    Tile &spawnTile = map.tiles[spawnPointIdx];


    // Spawn Initial builder
    Unit &builder = player->spawn(spawnTile);

    // If auto-spawn town hall mechanic is activated
    if(Config::getInstance().getMechanicTownHall()) {
        // build Town-Hall
        builder.build(0);
    }
    players.push_back(player);
    return *player;
}


GameMessage Game::serialize(GameMessage &gameMessage) {


    ///
    /// TILE DATA
    ///
    int numTiles = map.tiles.size();
    for (int i = 0; i < numTiles; i++) {
        Tile &t = map.tiles[i];

        gameMessage.add_tileids(t.id_);
        gameMessage.add_tileresources(t.resources);
        gameMessage.add_tileoccupant((t.occupant) ? t.occupant->id : 0);
    }
    ///
    /// PLAYER DATA
    ///
    int numUnits = 0;
    for(auto &p: players){
        numUnits += p->units.size();
    }

    int i = 0;
    for(auto &p: players){
        for(auto &u : p->units){
            Unit &ut = *u;

            //std::vector<BaseState *> stateList;
            //std::vector<Tile *> walking_path;


            gameMessage.add_unitsdirection(u->direction);
            gameMessage.add_unitsharvesttimer(u->harvestTimer);
            gameMessage.add_unitsharvestiterator(u->harvestIterator);
            gameMessage.add_unitslumbercarry(u->lumberCarry);
            gameMessage.add_unitsoilcary(u->oilCarry);
            gameMessage.add_unitsgoldcarry(u->goldCarry);
            gameMessage.add_unitsharvesttimer(u->harvestTimer);
            gameMessage.add_unitsharvestiterator(u->harvestIterator);
            gameMessage.add_unitsspawntimer(u->spawnTimer);
            gameMessage.add_unitsbuildtimer(u->buildTimer);
            gameMessage.add_unitscombattimer(u->combatTimer);
            gameMessage.add_unitswalkingtimer(u->walking_timer);
            gameMessage.add_unitstype(u->typeId);
            gameMessage.add_unitshealth(u->health);
            gameMessage.add_unitsids(u->id);
            gameMessage.add_unitstileid((u->tile) ? u->tile->id_: -1);
            gameMessage.add_unitsstate(u->state->id);

            if(u->combatTarget)
                gameMessage.add_unitscombattarget(u->combatTarget->id);
            else
                gameMessage.add_unitscombattarget(-1);
            if(u->buildEntity)
                gameMessage.add_unitbuildentity(u->buildEntity->id);
            else
                gameMessage.add_unitbuildentity(-1);
            if(u->walkingGoal)
                gameMessage.add_unitswalkinggoal(u->walkingGoal->id_);
            else
                gameMessage.add_unitswalkinggoal(-1);
            if(u->harvestTarget)
                gameMessage.add_unitsharvesttarget(u->harvestTarget->id_);
            else
                gameMessage.add_unitsharvesttarget(-1);
            if(u->spawnTile)
                gameMessage.add_unitsspawntileid(u->spawnTile->id_);
            else
                gameMessage.add_unitsspawntileid(-1);




            i++;
        }

        int pid = p->id_;
        int defeated = p->defeated;
        int statGoldGather = p->statGoldGather;
        int statLumberGather = p->statLumberGather;
        int statOilGather = p->statOilGather;
        int statUnitDamageTaken = p->statUnitDamageTaken;
        int statUnitDamageDone = p->statUnitDamageDone;
        int statUnitBuilt = p->statUnitBuilt;
        int statUnitMilitary = p->statUnitMilitary;

        int food = p->getFood();
        int consumedFood = p->getFoodConsumption();
        int gold = p->getGold();
        int lumber = p->getLumber();
        int oil = p->getOil();
        int unitCount = p->getUnitCount();

        gameMessage.add_playerid(pid);
        gameMessage.add_defeated(defeated);
        gameMessage.add_statgoldgather(statGoldGather);
        gameMessage.add_statlumbergather(statLumberGather);
        gameMessage.add_statoilgather(statOilGather);
        gameMessage.add_statunitdamagetaken(statUnitDamageTaken);
        gameMessage.add_statunitdamagedone(statUnitDamageDone);
        gameMessage.add_statunitbuilt(statUnitBuilt);
        gameMessage.add_statunitmilitary(statUnitMilitary);

        gameMessage.add_food(food);
        gameMessage.add_consumedfood(consumedFood);
        gameMessage.add_gold(gold);
        gameMessage.add_lumber(lumber);
        gameMessage.add_oil(oil);
        gameMessage.add_unitcount(unitCount);



    }

    ///
    /// Game Stuff
    ///
    int gameTicks = ticks;

    gameMessage.add_gameticks(gameTicks);


    return gameMessage;
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
	const int numTiles = map.tiles.size();


	for (int i = 0; i < numTiles; i++) {
		Tile &t = map.tiles[i];
		data["tileIds"].push_back(t.id_);
		data["tileResources"].push_back(t.resources);
		data["tileOccupant"].push_back((t.occupant) ? t.occupant->id : -1);
	}



	///
	/// PLAYER DATA
	///
	int numUnits = 0;
	for (auto &p : players) {
		numUnits += p->units.size();
	}

	int i = 0;
	for (auto &p : players) {
		for (auto &u : p->units) {
			Unit &ut = *u;

			//std::vector<BaseState *> stateList;
			//std::vector<Tile *> walking_path;

			data["unitsDirection"].push_back(u->direction);
			data["unitsHarvestTimer"] .push_back(u->harvestTimer);
			data["unitsHarvestIterator"] .push_back(u->harvestIterator);
			data["unitsLumberCarry"] .push_back(u->lumberCarry);
			data["unitsOilCarry"] .push_back(u->oilCarry);
			data["unitsGoldCarry"] .push_back(u->goldCarry);
			data["unitsSpawnTimer"] .push_back(u->spawnTimer);
			data["unitsBuildTimer"] .push_back(u->buildTimer);
			data["unitsCombatTimer"] .push_back(u->combatTimer);
			data["unitsWalkingTimer"] .push_back(u->walking_timer);
			data["unitsType"] .push_back(u->typeId);
			data["unitsHealth"] .push_back(u->health);
			data["unitsIds"] .push_back(u->id);
			data["unitsTileID"] .push_back((u->tile) ? u->tile->id_ : -1);
			data["unitsState"] .push_back(u->state->id);
			data["unitsPlayerID"].push_back(u->player_.id_);

			if (u->combatTarget)
				data["unitsCombatTarget"].push_back(u->combatTarget->id);
			else
				data["unitsCombatTarget"].push_back(-1);

			if (u->buildEntity)
				data["unitsBuildEntity"].push_back(u->buildEntity->id);
			else
				data["unitsBuildEntity"].push_back(-1);

			if (u->walkingGoal)
				data["unitsWalkingGoal"].push_back(u->walkingGoal->id_);
			else
				data["unitsWalkingGoal"].push_back(-1);

			if (u->harvestTarget)
				data["unitsHarvestTarget"].push_back(u->harvestTarget->id_);
			else
				data["unitsHarvestTarget"].push_back(-1);

			if (u->spawnTile)
				data["unitsSpawnTile"].push_back(u->spawnTile->id_);
			else
				data["unitsSpawnTile"].push_back(-1);




			i++;
		}

		int pid = p->id_;
		int defeated = p->defeated;
		int statGoldGather = p->statGoldGather;
		int statLumberGather = p->statLumberGather;
		int statOilGather = p->statOilGather;
		int statUnitDamageTaken = p->statUnitDamageTaken;
		int statUnitDamageDone = p->statUnitDamageDone;
		int statUnitBuilt = p->statUnitBuilt;
		int statUnitMilitary = p->statUnitMilitary;

		int food = p->getFood();
		int consumedFood = p->getFoodConsumption();
		int gold = p->getGold();
		int lumber = p->getLumber();
		int oil = p->getOil();
		int unitCount = p->getUnitCount();

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


void Game::load(GameMessage& gameMessage) {
    ///
    /// TILES
    ///
    for (int i = 0; i < map.tiles.size(); i++) {
        Tile &t = map.tiles[i];

        t.id_ = gameMessage.tileids(i);
        t.resources = gameMessage.tileresources(i);
        t.occupant = NULL;
    }

    ticks = gameMessage.gameticks(0);

    ///
    /// Player
    ///
    int nPlayers = gameMessage.playerid_size();
    for(auto &p : players){
        p->targetedUnit = NULL;
    }

    players.clear();
    std::map<int, std::shared_ptr<Unit>> unitsMap;
    int c = 0;
    for(int i = 0; i < nPlayers; i++) {
        std::shared_ptr<Player> player = std::shared_ptr<Player>(new Player(*this));
        player->id_ = gameMessage.playerid(i);
        player->name_ = "Player: " + std::to_string(player->id_);
        player->defeated = gameMessage.defeated(i);
        player->statGoldGather = gameMessage.statgoldgather(i);
        player->statLumberGather = gameMessage.statlumbergather(i);
        player->statOilGather = gameMessage.statoilgather(i);
        player->statUnitDamageTaken = gameMessage.statunitdamagetaken(i);
        player->statUnitDamageDone = gameMessage.statunitdamagedone(i);
        player->statUnitBuilt = gameMessage.statunitbuilt(i);
        player->statUnitMilitary = gameMessage.statunitmilitary(i);

        player->food = gameMessage.food(i);
        player->foodConsumption = gameMessage.consumedfood(i);
        player->gold = gameMessage.gold(i);
        player->lumber = gameMessage.lumber(i);
        player->oil = gameMessage.oil(i);

        int unitCount = gameMessage.unitcount(i);
        for(int j = 0; j < unitCount; j++) {
            std::shared_ptr<Unit> unit = player->createUnit(gameMessage.unitstype(c));


            unit->walking_timer = gameMessage.unitswalkingtimer(c);
            unit->combatTimer = gameMessage.unitscombattimer(c);
            unit->buildTimer = gameMessage.unitsbuildtimer(c);
            unit->spawnTimer = gameMessage.unitsspawntimer(c);
            unit->harvestIterator = gameMessage.unitsharvestiterator(c);
            unit->harvestTimer = gameMessage.unitsharvesttimer(c);
            unit->goldCarry = gameMessage.unitsgoldcarry(c);
            unit->oilCarry = gameMessage.unitsoilcary(c);
            unit->lumberCarry = gameMessage.unitslumbercarry(c);
            unit->direction = gameMessage.unitsdirection(c);

            int walkingGoalID = gameMessage.unitswalkinggoal(c);
            int harvestTargetID = gameMessage.unitsharvesttarget(c);
            int spawnTileID = gameMessage.unitsspawntileid(c);

            if(walkingGoalID == -1)
                unit->walkingGoal = NULL;
            else
                unit->walkingGoal = &map.tiles[walkingGoalID];

            if(harvestTargetID == -1)
                unit->harvestTarget = NULL;
            else
                unit->harvestTarget = &map.tiles[harvestTargetID];

            if(spawnTileID == -1)
                unit->spawnTile = NULL;
            else
                unit->spawnTile = &map.tiles[spawnTileID];




            unit->typeId = gameMessage.unitstype(c);
            unit->health = gameMessage.unitshealth(c);
            unit->id = gameMessage.unitsids(c);
            int tileId = gameMessage.unitstileid(c);
            if(tileId == -1){
                unit->tile = NULL;
            } else {
                unit->tile = &map.tiles[tileId];
                unit->tile->occupant = unit;
                unit->worldPosition = unit->tile->getPixelPosition();
            }
            unit->state = unit->stateManager.getByID(gameMessage.unitsstate(c));
            unit->current_state = unit->state->id;

            unitsMap[unit->id] = unit;
            player->addUnit(unit);
            c++;
        }

        players.push_back(player);

    }

    c = 0;
    for(int i = 0; i < nPlayers; i++) {
        int unitCount = gameMessage.unitcount(i);
        std::shared_ptr<Player> player = players[i];
        for (int j = 0; j < unitCount; j++) {
            std::shared_ptr<Unit> unit = player->units[j];


            int combatTargetID = gameMessage.unitscombattarget(c);
            int buildEntityID = gameMessage.unitbuildentity(c);

            if(combatTargetID == -1) {
                unit->combatTarget = NULL;
            } else {
                unit->combatTarget = unitsMap.at(combatTargetID);
            }
            if(buildEntityID == -1) {
                unit->buildEntity = NULL;
            } else {
                unit->combatTarget = unitsMap.at(buildEntityID);
            }

            c++;
        }
    }



}



void Game::deactivateGUI() {

}




