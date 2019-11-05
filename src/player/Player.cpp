//
// Created by Per-Arne on 24.02.2017.
//


#include "Player.h"
#include "../Game.h"
#include "../unit/UnitManager.h"
#include <algorithm>




Player::Player(Game &game, int id) :
    game_(game),
    config(game.config)
{
    id_ = id;

    setName("Player: " + std::to_string(id_));

    unitIndexes.reserve(1000);

    reset();

}


Unit& Player::spawn(Tile &spawnPoint) {
    // Spawn a builder


    Unit *unit = NULL;
    if (faction == 0) // Human
    {

        unit = &addUnit(Constants::Unit::Peasant);
    }
    else if (faction == 1) // Orc
    {
        unit = &addUnit(Constants::Unit::Peasant);
    }
    else {
        assert(false);
    }

    assert(unit && "Unit was null for some reason");
    unit->spawn(spawnPoint, unit->spawnDuration);
    unit->update();

    // Set targeted unit to newly spawned unit
    targetedUnitID = unit->id;


    return *unit;
}

void Player::update() {
    // TODO - remove
}

void Player::reset()
{
    faction = 0;
    gold = game_.config.startGold;
    lumber = game_.config.startWood;
    oil = game_.config.startOil;
    foodConsumption = 0;
    food = 1;
    defeated = false;

    sGatheredGold = 0;
    sGatheredLumber = 0;
    sGatheredOil = 0;
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

}



int Player::getFoodConsumption() {
    return foodConsumption;
}

int Player::getFood() {
    return food;
}

int Player::getGold() {
    return gold;
}

int Player::getOil() {
    return oil;
}

int Player::getLumber() {
    return lumber;
}

size_t Player::getUnitCount() {
    return unitIndexes.size();
}

int Player::getId() {
    return id_;
}

void Player::addGold(int n) {
    gold += n;
}

void Player::addLumber(int n) {
    lumber += n;
}

void Player::addOil(int n) {
    oil += n;
}

int Player::getScore() {
    uint8_t GOLD_VALUE = 2;
    uint8_t LUMBER_VALUE = 1;
    //uint8_t OIL_VALUE = 3;


    double_t gatherScore = (sGatheredGold * GOLD_VALUE + sGatheredLumber * LUMBER_VALUE) * .5;
    double_t builtScore = sUnitsCreated * 2;
    double_t damageScore = std::max(0.0, (sDamageDone * .6) - (sDamageTaken * .5));
    double_t unitScore = unitIndexes.size();


    double_t militaryScore = 0;
    double_t defenceScore = 0;


    return static_cast<uint32_t>(gatherScore + builtScore + unitScore + militaryScore + defenceScore + damageScore);

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

bool Player::isDefeated() {

    if (defeated){
        return true;
    }      // Defeated flag = True
    else if (num_peasant + num_footman + num_archer > 0){
        return false;
    }    // Have remaining units
    else if (
        (num_town_hall > 0 && gold >= 400) ||
        (num_barrack > 0 && gold >= 600)
    ){
        return false; // No units, but can purchase footman or peasant
    }


    defeated = true;
    return defeated;
}

bool Player::canPlace(Unit & builder, Unit & unit, Tile &_tile) {

    for (auto &tile : game_.tilemap.getTileArea(_tile, unit.width, unit.height)) {
        if (tile == builder.tile) // Ignore tile of the builder, this is handled in Unit::Build when builder despawns
            continue;

        if (!tile->isBuildable()) {
            return false;
        }

    }

    return true;
}

bool Player::canAfford(Unit & unit) {
    return gold >= unit.goldCost && lumber >= unit.lumberCost && oil >= unit.oilCost;

}

Unit& Player::addUnit(Constants::Unit unitType) {

    // Create new unit
    game_.units.push_back(UnitManager::constructUnit(unitType, *this));
    Unit& newUnit = game_.units.back();
    unitIndexes.push_back(newUnit.id);

    // Update the unit count
    UnitManager::updateUnitCount(*this, newUnit.typeId, 1);

    // Callback
    getGame()._onUnitCreate(newUnit);

    return newUnit;
}

void Player::removeOil(int n) {
    oil -= n;
}

void Player::removeLumber(int n) {
    lumber -= n;
}

void Player::removeGold(int n) {
    gold -= n;
}

void Player::setName(std::string _name) {
    name = _name;
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

    if (
            unitIndexes.empty() ||
            (
                    !getTargetedUnit() &&
                    actionID != Constants::Action::NextUnit &&
                    actionID != Constants::Action::PreviousUnit
                    )
                    )
    {
        // 1. No units to perform actions with OR
        // 2.1 Has selected a unit
        // 2.2 Action is NOT NextUnit
        // 2.3 Action is NoOT PreviousUnit
        return;
    }


    Unit *targetedUnit = getTargetedUnit();

    switch (actionID) {
        case Constants::Action::NextUnit:
            nextUnit();
            break;
        case Constants::Action::PreviousUnit:
            previousUnit();
            break;
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
        case Constants::Action::NoAction:
            break;
        default:
            // NO ACTION
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
