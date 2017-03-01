//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_UNIT_H
#define WARC2SIM_UNIT_H

#include <vector>
#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include <map>
#include <SFML/Graphics/Texture.hpp>

#include "../environment/Tile.h"
#include "../state/StateManager.h"
#include "../Config.h"


class Player;
class BaseState;



class Unit: public std::enable_shared_from_this<Unit>{
private:
    static int gId;
public:
    std::shared_ptr<Unit> getptr() {
        return shared_from_this();
    }

    // Graphics
    double animationInterval = .3 * 10;
    int animationTimer = 0;
    int animationIterator = 0;

    int id;
    int typeId;
    int health;
    int health_max;
    int current_state;
    int direction;


    int groundUnit;
    int waterUnit;

    int damageMin = -1;
    int damageMax = -1;
    int damageRange = -1;
    int damagePiercing = -1;
    int armor = -1;

    int lumberCarry = 0;
    int goldCarry = 0;
    int oilCarry = 0;
    int carryCapacity = 10;
    std::vector<std::shared_ptr<Unit>> buildInventory;
    int speed;
    int sight;
    int range;

    bool canHarvest;
    bool canAttack;
    bool canMove;
    bool military;
    bool structure;
    bool recallable = false;

    int lumberCost;
    int goldCost;
    int oilCost;

    int spawnDuration;
    Tile *spawnTile;

    int foodProduction;
    int foodConsumption;

    int width;
    int height;

    std::string name;


    Player &player_;


    // State attributes

    // Harvesting
    double harvestInterval = .5 * 10;
    int harvestTimer;
    int harvestIterator;
    Tile *harvestTarget;

    // Building
    int spawnTimer;
    std::shared_ptr<Unit> buildEntity;
    int buildTimer;

    // Combat
    std::shared_ptr<Unit> combatTarget;
    int combatTimer = 1000;
    double combatInterval = .1 * 10;

    // Walking
    int walking_timer = 0;
    std::vector<Tile *> walking_path;
    Tile *walkingGoal;
    double walking_interval = .001 * 10;


    Tile *tile = NULL;
    StateManager& stateManager;
    BaseState *state;

    std::vector<BaseState *> stateList;

    sf::Texture *testTexture;
    sf::Sprite *testSprite;


    Unit(Player &player);
    bool build(int idx);
    void spawn(Tile &x, int initValue);
    void despawn();
    void update();
    void enqueueState(BaseState &tate);
    void transitionState();
    void transitionState(BaseState &nextState);
    void setPosition(Tile &tile);
    void rightClick(Tile &tile);
    void move(Tile &targetTile);
    void attack(Tile &tile);
    void harvest(Tile &tile);
    int distance(Tile &tile);


    void clearTiles();

    std::shared_ptr<Unit> closestRecallBuilding();


    bool isDead();

    void afflictDamage(int dmg_);

    int getDamage(Unit &target);

    sf::Vector2f worldPosition;

    void setDirection(int newX, int newY);

    void moveRelative(int x, int y);
};

#endif //WARC2SIM_UNIT_H
