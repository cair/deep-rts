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


