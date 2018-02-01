//
// Created by Per-Arne on 23.02.2017.
//

#include "Game.h"
#include "unit/UnitManager.h"


std::unordered_map<int, Game*> Game::games;
Game::Game():
		map(Tilemap(Config::getInstance().getMapName(), *this))
{
    players.reserve(Constants::MAX_PLAYERS);
	units.reserve(Constants::MAX_PLAYERS * Constants::MAX_UNITS);

    // State vector
    // 0 - Environment
    // 1 - Unit/Building Type
    // 2 - Unit/Building Player
    // 3 - Unit/Building Health
    int c = 0;
    if (Config::getInstance().state_environment) {
        state.emplace_back(std::vector<float>(map.MAP_WIDTH * map.MAP_HEIGHT));
        state_environment_idx = c++;
    }
    if (Config::getInstance().state_unit_player) {
        state.emplace_back(std::vector<float>(map.MAP_WIDTH * map.MAP_HEIGHT));
        state_unit_player_idx = c++;
    }
    if (Config::getInstance().state_unit_health) {
        state.emplace_back(std::vector<float>(map.MAP_WIDTH * map.MAP_HEIGHT));
        state_unit_health_idx = c++;
    }
    if (Config::getInstance().state_unit_type){
        state.emplace_back(std::vector<float>(map.MAP_WIDTH * map.MAP_HEIGHT));
        state_unit_type_idx = c++;
    }

    setMaxFPS(Config::getInstance().getFPS());
    setMaxUPS(Config::getInstance().getUPS());

	id = static_cast<uint8_t>(games.size());
	games[id] = this;

    // Update timings
    tick();
    timerInit();

}


void Game::setMaxFPS(uint32_t fps_){
	max_fps = fps_;
	_render_interval = std::chrono::nanoseconds(1000000000 /  max_fps);


}

void Game::setMaxUPS(uint32_t ups_){
	max_ups = ups_;
	_update_interval = std::chrono::nanoseconds(1000000000 /  max_ups);


}

void Game::start(){
    timerInit();
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
	for (auto &tile : map.getTiles()) {
		tile.reset();
	}

	// Reset all players
	for (auto &player : players) {
		player.reset();
		spawnPlayer(player);
	}

	// Reset tick counter
	ticks = 0;

    // Increase epsiode counter
	episode += 1;

    terminal = false;

    _reset();
    _onEpisodeStart();



}

void Game::update(){

    if (running && !terminal &&  now >= _update_next) {

        // Iterate through all units
        for(auto &unit : units) {
            if (unit.removedFromGame) continue;		// Skip unit that is removed from game
            unit.update();
        }

        // Iterate through all players
        for (auto &p : players) {
            p.update();
        }

        _update();

        // Update Counters and statistics
        _update_next += _update_interval;
        _update_delta += 1;
        ticks += 1;
    }

}


void Game::render(){
    if (now >= _render_next) {
        _render();


        _render_next += _render_interval;
        _render_delta += 1;
    }}

void Game::_render(){}
void Game::_caption(){}
void Game::_update(){}
void Game::_reset() {}


void Game::caption() {
    if (now >= this->_stats_next) {
        _caption();
        if (consoleCaptionEnabled) {
            std::cout << "[FPS=" << this->currentFPS << ", UPS=" << this->currentUPS << "]" << std::endl;
        }



        currentFPS = _render_delta;
        currentUPS = _update_delta;
        _render_delta = 0;
        _update_delta = 0;
        _stats_next += std::chrono::nanoseconds(1000000000);    // 1 Second
    }


}


void Game::tick() {
    now = std::chrono::high_resolution_clock::now();
}

void Game::timerInit() {
    tick();
    _render_next = now + _render_interval;
    _update_next = now + _update_interval;
    _stats_next = now + std::chrono::nanoseconds(0);
}



Tilemap &Game::getMap() {
	return map;
}



std::vector<std::vector<float>> Game::getState(){
    int i = 0;
    for(auto tile : map.getTiles()){

        float tileId = tile.getTypeId();
        float uType = 0;
        float uPlayer = 0;
        float uHealth = 0;
        if (tile.hasOccupant()) {
            auto occupant = tile.getOccupant();
            uType = occupant->typeId;
            uPlayer = occupant->player_.getId() + 1; // TODO id should be integrated maybe? player.id = id + 1
            uHealth = occupant->health / occupant->health_max;
        }

        if (Config::getInstance().state_environment) state[state_environment_idx][i] = tileId;
        if (Config::getInstance().state_unit_type) state[state_unit_type_idx][i] = uType;
        if (Config::getInstance().state_unit_player) state[state_unit_player_idx][i] = uPlayer;
        if (Config::getInstance().state_unit_health) state[state_unit_health_idx][i] = uHealth;

        i++;
    }
    return state;
}


Game * Game::getGame(uint8_t id)
{
	Game *g = games.at(id);
	assert(g);
	return g;
}


bool Game::isTerminal(){

    // Skip check if terminal is already set
    if(terminal){
        return terminal;
    }

	int c = 0;
	for(auto &p : players) {
		if(p.isDefeated()){
			c++;
		}
	}

	bool isTerminal = (c == 1);
	terminal = isTerminal;

    _onEpisodeEnd();
	return terminal;
}


Player *Game::addPlayer() {
	players.emplace_back(*this, players.size());
	Player &player = players.back();

	spawnPlayer(player);
	return &player;
}

void Game::spawnPlayer(Player &player) {
	// Retrieve spawn_point

    if(map.spawnTiles.size() < players.size()){
        throw std::runtime_error(std::string("Failed to spawn player, There are not enough spawn tiles!"));
    }

	int spawnPointIdx = map.spawnTiles[player.getId()];

    auto spawnTile = map.getTiles()[spawnPointIdx];

	// Spawn Initial builder
	Unit &builder = player.spawn(spawnTile);

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

size_t Game::getWidth() const{
    return map.MAP_WIDTH;
}

size_t Game::getHeight() const{
    return map.MAP_HEIGHT;
}


uint32_t Game::getMaxFPS() const {
    return max_fps;
}

uint32_t Game::getMaxUPS() const {
    return max_ups;
}

uint32_t Game::getFPS() const {
    return currentFPS;
}

uint32_t Game::getUPS() const {
    return currentUPS;
}

uint16_t Game::getEpisode() const {
    return episode;
}

uint64_t Game::getTicks() const{
    return this->ticks;
}

uint64_t Game::getGameDuration() const{
    return this->ticks / getTicksModifier();
}

uint8_t Game::getTicksModifier() const {
    return Config::getInstance().getTickModifier();
}

uint8_t Game::getId() const {
    return id;
}

void Game::_onUnitCreate(Unit& unit) {
    //DEBUG("Unit Created: " + unit.name + " | " + unit.player_.getName() + "\n");
}


void Game::_onEpisodeStart() {
    //DEBUG("Episode " + std::to_string(episode) + " started.\n");
}

void Game::_onEpisodeEnd() {
    //DEBUG("Episode " + std::to_string(episode) + " ended.\n");
}

void Game::_onUnitDestroy(Unit& unit) {
    //DEBUG("Unit Destroyed: " + unit.name + " | " + unit.player_.getName() + "\n");
}



void Game::_onTileDeplete(Tile &){}








