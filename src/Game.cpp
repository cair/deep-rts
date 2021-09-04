//
// Created by Per-Arne on 23.02.2017.
//

#include "Game.h"
#include "PyGUI.h"


Game::Game(const std::string& map_file):
        config(Config::defaults()),
        map(map_file),
        state(xt::xarray<double>::shape_type{
            static_cast<unsigned long>(map.MAP_WIDTH),
            static_cast<unsigned long>(map.MAP_HEIGHT),
            10
        }), // Wait until map is loaded
        tilemap(map, *this),
        stateManager(*this) {
    _internalInit();
}


Game::Game(const std::string& map_file, Config config):
        config(config),
        map(map_file),
        state(xt::xarray<double>::shape_type{
                static_cast<unsigned long>(map.MAP_WIDTH),
                static_cast<unsigned long>(map.MAP_HEIGHT),
                10
        }), // Wait until map is loaded
        tilemap(map, *this),
        stateManager(*this) {
    _internalInit();
}
Game::~Game() = default;

void Game::_internalInit(){
    players.reserve(Constants::MAX_PLAYERS);
    units.reserve(Constants::MAX_PLAYERS * Constants::MAX_UNITS);

    setMaxFPS(60);
    setMaxUPS(10);
    timerInit();

    if(config.gui) {
        GUI = new PyGUI(*this);
        tilemap.reset();
    }
}

void Game::setMaxFPS(int fps_){
    max_fps = fps_;
    if(fps_ < 0){
        _render_interval = std::chrono::nanoseconds(0);
        return;
    }
    _render_interval = std::chrono::nanoseconds(1000000000 /  max_fps);
}

void Game::setMaxUPS(int ups_){
    max_ups = ups_;
    if(ups_ < 0){
        _update_interval = std::chrono::nanoseconds(0);
        return;
    }
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
    // Reset tilemap
    tilemap.reset();

	// Remove all units
	unitsNameMap.clear();
	units.clear();


    // Reset all players
    for (auto &player : players) {
        player.reset();
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
    tick();

    if (running && !terminal &&  now >= _update_next) {

        // Iterate through all units
        for(auto &unit : units) {
            if (unit.removedFromGame){
                unitsNameMap.erase(unit.nameID);
                continue;
            }		// Skip unit that is removed from game
            unit.update();
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


void Game::caption() {
    if (now >= this->_stats_next) {
        _caption();

        if (config.consoleCaptionEnabled) {
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


bool Game::isTerminal(){

    if(!config.terminalSignal) {
        return false;
    }

    // Skip check if terminal is already set
    if(terminal){
        return terminal;
    }

    int c = 0;
    for(auto &p : players) {
        if(p.playerState() == Constants::PlayerState::Defeat){
            c++;
        }
    }

    bool isTerminal = (c == 1);

    terminal = isTerminal;

    _onEpisodeEnd();
    return terminal;
}


Player &Game::addPlayer() {

    players.emplace_back(*this, players.size());
    Player &player = players.back();

    // Set initial
    if(this->selectedPlayer == nullptr){
        this->setSelectedPlayer(player);
    }

    return player;
}

void Game::insertPlayer(Player& player) {

    players.push_back(player);

    // Set initial
    if(this->selectedPlayer == nullptr){
        this->setSelectedPlayer(player);
    }
}



Unit & Game::getUnit(int idx)
{
    assert((idx >= 0 && idx < (int)units.size()) && "getUnit(idx) failed. Index not in range!");
    return units[idx];
}

Unit* Game::getUnitByNameID(const std::string& nameID) {
    auto f = unitsNameMap.find(nameID);
    if (f == unitsNameMap.end() ) {
        return nullptr;
    }

    return f->second;
}

int Game::getWidth() const{
    return map.MAP_WIDTH;
}

int Game::getHeight() const{
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

uint32_t Game::getEpisode() const {
    return episode;
}

uint64_t Game::getTicks() const{
    return this->ticks;
}

uint64_t Game::getGameDuration() const{
    return this->ticks / getTicksModifier();
}

long Game::getTicksModifier() const {
    return config.tickModifier;
}

uint32_t Game::getId() const {
    return id;
}

void Game::setSelectedPlayer(Player &player) {
    this->selectedPlayer = &player;

}

void Game::_onUnitCreate(Unit& unit) {
    (void)(unit);
    //DEBUG("Unit Created: " + unit.name + " | " + unit.player_.getName() + "\n");
}


void Game::_onEpisodeStart() {
    //DEBUG("Episode " + std::to_string(episode) + " started.\n");
}

void Game::_onEpisodeEnd() {
    //DEBUG("Episode " + std::to_string(episode) + " ended.\n");
}

void Game::_onUnitDestroy(Unit& unit) {
    (void)(unit);
    //DEBUG("Unit Destroyed: " + unit.name + " | " + unit.player_.getName() + "\n");
}

void Game::_onTileChange(Tile& tile){
    if(GUI) {
        GUI->onTileChange(tile);
    }
}


void Game::_onResourceGather(Tile& tile, Unit& unit) {
    (void)(tile);
    (void)(unit);
}
void Game::_onResourceDepleted(Tile& tile, Unit& unit) {
    (void)(tile);
    (void)(unit);
}


void Game::_render(){
    if(GUI) {
        GUI->view();
    }
}

void Game::_caption(){}
void Game::_update(){}
void Game::_reset() {}
