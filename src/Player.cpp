//
// Created by Per-Arne on 24.02.2017.
//


#include "Player.h"
#include "Game.h"
#include "UnitManager.h"
#include <algorithm>
#include <utility>
#include <string>
using namespace DeepRTS;

Player::Player(Game &game, int id):
    game_(game),
    faction(0),
    playerState(Constants::PlayerState::Playing),
    config(game.config)
{
    if(id < 0){
        // Units created with a negative number should not be initialised for in-game purposes
        return;
    }

    id_ = id;
    setName("Player: " + std::to_string(id_));
    unitIndexes.reserve(1000);
    reset();

}


Unit& Player::spawn(Tile &spawnPoint) {
    // Spawn a builder


    Unit *unit;
    switch (faction) {
        case 0: // Human
            unit = &addUnit(Constants::Unit::Peasant);
            break;
        case 1: // Orc
            unit = &addUnit(Constants::Unit::Peasant); // TODO
            break;
        default:
            unit = nullptr;

    }
    assert(unit && "Unit was null for some reason");

    return spawnUnit(*unit, spawnPoint, 1);
}

Unit& Player::spawnUnit(Unit& unit, Tile& tile, int select = 1){

    unit.spawn(tile, unit.spawnDuration);
    unit.update();

    // Set targeted unit to newly spawned unit
    targetedUnitID = unit.id;

    return unit;
}


void Player::reset()
{
    faction = 0;  // TODO hardcoded
    gold = game_.config.startGold;
    lumber = game_.config.startLumber;
    stone = game_.config.startStone;
    foodConsumption = 0;
    food = game_.config.startFood;
    playerState = Constants::PlayerState::Playing;

    sGatheredGold = 0;
    sGatheredLumber = 0;
    sGatheredStone = 0;
    sDamageDone = 0;
    sDamageTaken = 0;
    sUnitsCreated = 0;

    num_footman = 0;
    num_peasant = 0;
    num_archer = 0;

    num_farm = 0;
    num_barrack = 0;
    num_town_hall = 0;

    unitIndexes.clear();
    targetedUnitID = -1;

    spawnPlayer();

}


void Player::spawnPlayer() {
    // Retrieve spawn_point

    auto spawnTile = game_.tilemap.getSpawnTile(getId());

    // Spawn Initial builder
    Unit &builder = spawn(spawnTile);

    // If auto-spawn town hall mechanic is activated

    if (config.instantTownHall) {
        // build Town-Hall
        builder.build(0, true);
    }

}

void Player::spawnUnitNearSpawnPoint(Constants::Unit unitType) {
    auto playerSpawnTile = game_.tilemap.getSpawnTile(getId());
    auto neighbors = game_.tilemap.neighbors(playerSpawnTile, Constants::Pathfinding::Walkable);
    if (neighbors.empty()) {
        std::cout << "No more space to build units!";
        return;
    }

    auto unitSpawnTile = neighbors[0];
    auto unit = addUnit(unitType);
    spawnUnit(unit, *unitSpawnTile, 1);
}


[[maybe_unused]] int Player::getFoodConsumption() const {
    return foodConsumption;
}

int Player::getFood() const {
    return food;
}

[[maybe_unused]] int Player::getGold() const {
    return gold;
}

[[maybe_unused]] int Player::getStone() const {
    return stone;
}

[[maybe_unused]] int Player::getLumber() const {
    return lumber;
}

int Player::getUnitCount() const {
    return unitIndexes.size();
}

int Player::getId() const {
    return id_;
}

void Player::addGold(int n) {
    gold += n;
}

void Player::addLumber(int n) {
    lumber += n;
}

void Player::addStone(int n) {
    stone += n;
}

double Player::getScore() const {
    auto GOLD_VALUE = 2.0;
    auto LUMBER_VALUE = 1.0;
    auto STONE_VALUE = 3.0;

    auto gatherScore = (sGatheredGold * GOLD_VALUE + sGatheredLumber * LUMBER_VALUE + sGatheredStone * STONE_VALUE) * .5;
    auto builtScore = sUnitsCreated * 2.0;
    auto damageScore = std::max(0.0, (sDamageDone * .6) - (sDamageTaken * .5));
    auto unitScore = double(unitIndexes.size());

    auto militaryScore = 0.0;
    auto defenceScore = 0.0;

    return gatherScore + builtScore + unitScore + militaryScore + defenceScore + damageScore;

}

void Player::removeUnit(Unit & unit) {
    for (auto &p : game_.players) {
        if (p.targetedUnitID == unit.id) {
            p.targetedUnitID = -1;
        }
    }

    // Update player counters
    UnitManager::updateUnitCount(*this, unit.typeId, -1);

    // Set removed flag
    unit.removedFromGame = true;

    auto pos = std::find(game_.units.begin(), game_.units.end(), unit.id) - game_.units.begin();
    unitIndexes.erase(std::remove(unitIndexes.begin(), unitIndexes.end(), pos), unitIndexes.end());

    // Callback
    getGame()._onUnitDestroy(unit);
}


Constants::PlayerState Player::getState() const{
    return playerState;
}

Constants::PlayerState Player::evaluatePlayerState() const {

    if (playerState == Constants::PlayerState::Defeat){
        return Constants::PlayerState::Defeat;
    }

    auto hasTownHallAndSufficientGold = (num_town_hall > 0 && gold >= 400);
    auto hasBarracksAndSufficientGold = (num_barrack > 0 && gold >= 600);
    auto hasRemainingUnits = unitIndexes.size() > +0;
    auto hasMovableUnits = num_archer + num_footman + num_peasant;


    if (hasTownHallAndSufficientGold || hasBarracksAndSufficientGold || (hasMovableUnits && hasRemainingUnits)){
        return Constants::PlayerState::Playing;
    }


    return Constants::PlayerState::Defeat;
}

void Player::setState(Constants::PlayerState state){
    playerState = state;
}

bool Player::canPlace(Unit & builder, Unit & unit, Tile &_tile) {

    auto tileArea = game_.tilemap.getTileArea(_tile, unit.width, unit.height);

    for (auto &tile :tileArea) {
        if (tile == builder.tile) // Ignore tile of the builder, this is handled in Unit::Build when builder despawns
            continue;

        if (!tile->isBuildable()) {
            return false;
        }

    }

    return true;
}

bool Player::canAfford(Unit & unit) const {
    return gold >= unit.goldCost && lumber >= unit.lumberCost && stone >= unit.stoneCost;

}

Unit& Player::addUnit(Constants::Unit unitType) {

    // Create new unit
    game_.units.push_back(UnitManager::constructUnit(unitType, *this));
    Unit& newUnit = game_.units.back();
    unitIndexes.push_back(newUnit.id);
    game_.unitsNameMap[newUnit.nameID] = &newUnit;

    // Update the unit count
    UnitManager::updateUnitCount(*this, newUnit.typeId, 1);

    // Callback
    getGame()._onUnitCreate(newUnit);

    return newUnit;
}

void Player::removeStone(int n) {
    stone -= n;
}

void Player::removeLumber(int n) {
    lumber -= n;
}

void Player::removeGold(int n) {
    gold -= n;
}

void Player::setName(std::string _name) {
    name = std::move(_name);
}


int Player::_getNextPrevUnitIdx() {

    if (unitIndexes.empty()) {
        return -1;
    }

    if (targetedUnitID == -1) {
        return 0;
    }

    uint16_t idx = 0;
    for (auto &uIDX : unitIndexes) {
        if (targetedUnitID == uIDX) {
            return idx;
        }
        idx++;
    }

    return -1;
}

void Player::nextUnit() {
    //assert(unitIndexes.size() == 0 && "AI is attempting to play on a terminal state!");

    // Get which index in the unitIndexes to lookup
    int idx = _getNextPrevUnitIdx();

    // Just return if no next unit is returned
    if (idx == -1)
        return;

    // Increment since we want Next unit
    idx++;

    // Retrieve which index it has
    uint16_t unitIndex = unitIndexes[idx % unitIndexes.size()];
    //std::cout << static_cast<int>(unitIndex) << " -N- " << unitIndexes.size() << std::endl;
    // Get unit from the game vector
    targetedUnitID = unitIndex;
}

void Player::previousUnit() {
    //assert(unitIndexes.size() == 0 && "AI is attempting to play on a terminal state!");

    // Get which index in the unitIndexes to lookup
    int idx = _getNextPrevUnitIdx();


    // Just return if no next unit is returned
    if (idx == -1)
        return;

    // Increment since we want Previous unit
    idx--;

    // Retrieve which index it has
    uint16_t unitIndex = unitIndexes[idx % unitIndexes.size()];
    //std::cout << static_cast<int>(unitIndex) << " -P- " << unitIndexes.size() << std::endl;

    // Get unit from the game vector
    targetedUnitID = unitIndex;
}

Unit *Player::getTargetedUnit() {
    if(targetedUnitID == -1) {
        return nullptr;
    }

    return &game_.units[targetedUnitID];
}

void Player::leftClick(int x, int y) {
    Tile& tile = getGame().tilemap.getTile(x, y);

    if(tile.getOccupant()) {
        setTargetedUnitID(tile.getOccupant()->id);
    }else {
        setTargetedUnitID(-1);
    }
}


void Player::rightClick(int x, int y) {
    if(x < 0 || y < 0 || x > game_.map.MAP_WIDTH || y > game_.map.MAP_HEIGHT) {
        return;
    }
    Tile &t = game_.tilemap.getTile(x, y);
    Unit* targetedUnit = getTargetedUnit();
    if(!targetedUnit) {
        return;
    }
    targetedUnit->rightClick(t);

}

void Player::do_manual_action(int manual_action_id, int x, int y){
    if(manual_action_id == 0){
        // NOOP
    }else if(manual_action_id == 1){
        // Left click
        leftClick(x, y);
    }else if(manual_action_id == 2){
        // Right click
        rightClick(x, y);
    }

}

void Player::do_action(int actionID) {

    if(actionID > Constants::ACTION_MAX || actionID < Constants::ACTION_MIN){
        throw std::runtime_error("The action '" + std::to_string(actionID) + "' is out of bounds. Actions must be between '" + std::to_string(Constants::ACTION_MIN) + "' and '" + std::to_string(Constants::ACTION_MAX) + "'.");
    }

    // If there is no units on the map for the player, there is no action to do. therefore:
    // 1. Check if unitIndex list is empty. if this is the case, return.
    if(unitIndexes.empty()) {
        return;
    }

    // These actions can always be performed regardless of targeted unit or not.
    // 1 Action is NextUnit
    // 2 Action is PreviousUnit
    // 3 Action is no-action
    if(actionID == Constants::Action::NextUnit){
        nextUnit();
        return;
    }else if(actionID == Constants::Action::PreviousUnit){
        previousUnit();
        return;
    }else if(actionID == Constants::Action::NoAction) {
        return;
    }

    Unit *targetedUnit = getTargetedUnit();

    // Ensure that:
    // 1. Player has selected a unit
    if(!getTargetedUnit()){
        return;
    }

    switch (actionID) {
        //case Constants::Action::NextUnit:
        //    break;
        //case Constants::Action::PreviousUnit:
        //    break;
        case Constants::Action::MoveUpRight:
            targetedUnit->tryMove(-1, 1);
            break;
        case Constants::Action::MoveUpLeft:
            targetedUnit->tryMove(-1, -1);
            break;
        case Constants::Action::MoveDownRight:
            targetedUnit->tryMove(1, 1);
            break;
        case Constants::Action::MoveDownLeft:
            targetedUnit->tryMove(1, -1);
            break;
        case Constants::Action::MoveUp:
            targetedUnit->tryMove(0, -1);
            break;
        case Constants::Action::MoveDown:
            targetedUnit->tryMove(0, 1);
            break;
        case Constants::Action::MoveLeft:
            targetedUnit->tryMove(-1, 0);
            break;
        case Constants::Action::MoveRight:
            targetedUnit->tryMove(1, 0);
            break;
        case Constants::Action::Attack:
            targetedUnit->tryAttack();
            break;
        case Constants::Action::Harvest:
            targetedUnit->tryHarvest();
            break;
        case Constants::Action::Build0:
            targetedUnit->build(0);
            break;
        case Constants::Action::Build1:
            targetedUnit->build(1);
            break;
        case Constants::Action::Build2:
            targetedUnit->build(2);
            break;
        //case Constants::Action::NoAction:
        //    break;
        default:
            break;
    }






}


Game &Player::getGame() const {
    return game_;
}

const std::string &Player::getName() const {
    return name;
}

int Player::getTargetedUnitID() const {
    return targetedUnitID;
}

void Player::setTargetedUnitID(int _targetedUnitID) {
    Player::targetedUnitID = _targetedUnitID;
}
