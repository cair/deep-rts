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


class Player;
class BaseState;



class Unit{
private:
    static int gId;
public:
    // Graphics
    double animation_interval;
    int animation_timer;
    int animation_iterator;

    int id;
    int type_id;
    int health;
    int health_max;
    int current_state;
    int direction;


    int groundUnit;
    int waterUnit;

    int damage_min = -1;
    int damage_max = -1;
    int damage_range = -1;
    int damage_piercing = -1;
    int armor = -1;

    int lumberCarry = 0;
    int goldCarry = 0;
    int oilCarry = 0;
    int harvestInventory;
    int harvestCapacity;
    std::vector<Unit> buildInventory;
    int speed;
    int sight;
    int range;

    bool canHarvest;
    bool canAttack;
    bool canMove;
    bool military;
    bool structure;

    int lumberCost;
    int goldCost;

    int spawnDuration;

    int foodProduction;
    int foodConsumption;

    int width;
    int height;

    std::string name;


    Player &player_;


    // State attributes

    // Harvesting
    double harvest_interval;
    int harvest_timer;
    int harvest_iterator;
    int harvest_target_x;
    int harvest_target_y;

    // Building
    int spawnTimer;
    int build_target_id;
    int build_entity_id;

    // Combat
    int combat_target_id;
    int combat_timer;
    double combat_interval;

    // Walking
    int walking_timer;
    std::vector<std::pair<int, int>> walking_path;
    int walking_goal_x;
    int walking_goal_y;
    double walking_interval;


    Tile *tile = NULL;
    StateManager stateManager;
    BaseState *state;

    std::vector<BaseState *> stateList;

    sf::Texture *testTexture;
    sf::Sprite *testSprite;


    Unit(Player &player);
    bool build(int idx);
    void spawn(Tile &x);
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



};

#endif //WARC2SIM_UNIT_H
