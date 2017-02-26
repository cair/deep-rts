//
// Created by Per-Arne on 23.02.2017.
//

#include "Game.h"

#include "Config.h"
#include "graphics/GUI.h"


Game::Game(int n_players):
        map(MapLoader::load("contested-4v4.json"))
{
    // Definitions
    this->n_players = n_players;
    this->_render_interval = Config::getInstance().getUPSInterval();
    this->_update_interval = Config::getInstance().getFPSInterval();

    this->gui = new GUI(*this);


}

void Game::create_players(){


    for (int i = 0; i < n_players; i++) {

        Player *player = new Player(*this);
        players.push_back(player);

        // Retrieve spawn_point
        int spawnPointIdx = map.spawnTiles[i];
        Tile &spawnTile = map.tiles[spawnPointIdx];

        // Spawn Initial builder
        Unit &builder = player->spawn(spawnTile);

        // If auto-spawn town hall mechanic is activated
        if(Config::getInstance().getMechanicTownHall()) {
            // build Town-Hall
            builder.build(0);
        }


    }

}

void Game::load_players(){

}

void Game::start(){
    this->running = true;
}

void Game::stop(){
    this->running = false;
}


void Game::loop() {

    clock_t now = clock();
    this->_caption_next = now + this->_caption_interval;
    this->_render_next= now + this->_render_interval;
    this->_update_next = now + this->_update_interval;

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

        if (now >= this->_caption_next) {
            // Caption
            std::cout << "[FPS=" << this->fps << ", UPS=" << this->ups<< "]" << std::endl;
            this->_caption_next += this->_caption_interval;
        }

        if (now >= this->_stats_next) {
            this->fps = this->_update_delta;
            this->ups = this->_render_delta;
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

