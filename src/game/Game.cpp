//
// Created by Per-Arne on 23.02.2017.
//

#include "Game.h"

#include "Config.h"
#include "graphics/GUI.h"
#include "unit/Peasant.h"
#include "unit/TownHall.h"
#include "lib/ColorConverter.h"
#include "graphics/Animation.h"
#include "action/BaseAction.h"
#include "proto/GameMessage.pb.h"

Game::Game(int n_players):
        map(Tilemap("contested-4v4.json"))
{
    // Definitions
    n_players = n_players;
    setFPS(Config::getInstance().getFPS());
    setUPS(Config::getInstance().getUPS());
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

            for(auto *p : this->players) {
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

    bool isTerminal = (c == 1);
    terminal = isTerminal;

    return terminal;
}

void Game::addAction(std::shared_ptr<BaseAction> action) {
    executedActions.push_back(action);
}

Player &Game::addPlayer() {
    Player *player = new Player(*this);


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

/*
GameMessage Game::serialize(Player *povPlayer) {
    /// Pov player is AI's player
    GameMessage gameMessage = GameMessage();

    ///
    /// TILE DATA
    ///
    int numTiles = map.tiles.size();
    int tileIDs[numTiles];
    int tileResources[numTiles];
    int tileOccupant[numTiles];
    for (int i = 0; i < numTiles; i++) {
        Tile &t = map.tiles[i];

        gameMessage.add_tileids(t.id);
        gameMessage.add_tileresources(t.resources);
        gameMessage.add_tileoccupant((t.occupant) ? t.occupant->id : 0);
    }
    ///
    /// PLAYER DATA
    ///
    int numUnits = 0;
    for(auto p: players){
        numUnits += p->units.size();
    }
    int unitsIDS[numUnits];
    int unitsType[numUnits];
    int unitsHealth[numUnits];
    int unitsTileID[numUnits];
    int unitsState[numUnits];
    int i = 0;
    for(auto p: players){
        for(auto u : p->units){

            gameMessage.add_unitstype(u->typeId);
            gameMessage.add_unitshealth(u->health);
            gameMessage.add_unitsids(u->id);
            gameMessage.add_unitstileid((u->tile) ? u->tile->id: 0);
            gameMessage.add_unitsstate(u->current_state);

            i++;
        }
    }

    ///
    /// Player stuff
    ///
    ///
    int defeated = povPlayer->defeated;
    int statGoldGather = povPlayer->statGoldGather;
    int statLumberGather = povPlayer->statLumberGather;
    int statOilGather = povPlayer->statOilGather;
    int statUnitDamageTaken = povPlayer->statUnitDamageTaken;
    int statUnitDamageDone = povPlayer->statUnitDamageDone;
    int statUnitBuilt = povPlayer->statUnitBuilt;
    int statUnitMilitary = povPlayer->statUnitMilitary;

    int food = povPlayer->getFood();
    int consumedFood = povPlayer->getFoodConsumption();
    int gold = povPlayer->getGold();
    int lumber = povPlayer->getLumber();
    int oil = povPlayer->getOil();
    int unitCount = povPlayer->getUnitCount();

    gameMessage.set_defeated(defeated);
    gameMessage.set_statgoldgather(statGoldGather);
    gameMessage.set_statlumbergather(statLumberGather);
    gameMessage.set_statoilgather(statOilGather);
    gameMessage.set_statunitdamagetaken(statUnitDamageTaken);
    gameMessage.set_statunitdamagedone(statUnitDamageDone);
    gameMessage.set_statunitbuilt(statUnitBuilt);
    gameMessage.set_statunitmilitary(statUnitMilitary);

    gameMessage.set_food(food);
    gameMessage.set_consumedfood(consumedFood);
    gameMessage.set_gold(gold);
    gameMessage.set_lumber(lumber);
    gameMessage.set_oil(oil);
    gameMessage.set_unitcount(unitCount);


    ///
    /// Game Stuff
    ///
    int gameTicks = ticks;

    gameMessage.set_gameticks(gameTicks);


    return gameMessage;







}
 */


