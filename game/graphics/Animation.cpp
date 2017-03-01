//
// Created by Per-Arne on 25.02.2017.
//

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include "Animation.h"
#include "../Constants.h"


void Animation::add(const int unit, const int state, const int direction, int x, int y, int w, int h, int wrs, int hrs, sf::Texture* texture, bool flip) {

    std::shared_ptr<sf::Sprite> sprite = std::shared_ptr<sf::Sprite>(new sf::Sprite());
    sprite->setTexture(*texture);
    if(flip)
        w * -1;
    sprite->setTextureRect(sf::IntRect(x, y, w, h));
    sprite->setScale((double)wrs/w,(double)hrs/h);
    std::cout << sprite->getScale().x << std::endl;
    Animation::sprites[unit][state][direction].push_back(sprite);



}

void Animation::setup() {
    peasant = std::shared_ptr<sf::Texture>(new sf::Texture());
    peasant->loadFromFile("data/textures/human/peasant.png");

    buildings = std::shared_ptr<sf::Texture>(new sf::Texture());
    buildings->loadFromFile("data/textures/human/buildings.png");


    setupPeasant();
    setupTownHall();


}

void Animation::setupPeasant(){
    int P = Constants::Unit_Peasant;


    Animation::add(P, Constants::State_Walking, Constants::D_Left, 85, 4,  40, 40, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State_Walking, Constants::D_Left, 85, 42,  40, 40, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State_Walking, Constants::D_Left, 85, 82,  40, 40, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State_Walking, Constants::D_Left, 85, 118,  40, 40, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State_Walking, Constants::D_Left, 85, 155,  40, 40, 32, 32, &*peasant, true);

    Animation::add(P, Constants::State_Walking, Constants::D_Up, 15, 4,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_Up, 15, 42,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_Up, 15, 82,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_Up, 15, 118,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_Up, 15, 155,  40, 40, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State_Walking, Constants::D_Right, 85, 4,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_Right, 85, 42,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_Right, 85, 82,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_Right, 85, 118,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_Right, 85, 155,  40, 40, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State_Walking, Constants::D_Down, 164, 4,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_Down, 164, 42,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_Down, 164, 82,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_Down, 164, 118,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_Down, 164, 155,  40, 40, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State_Walking, Constants::D_UpLeft, 50, 4,  40, 40, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State_Walking, Constants::D_UpLeft, 50, 42,  40, 40, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State_Walking, Constants::D_UpLeft, 50, 82,  40, 40, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State_Walking, Constants::D_UpLeft, 50, 118,  40, 40, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State_Walking, Constants::D_UpLeft, 50, 155,  40, 40, 32, 32, &*peasant, true);

    Animation::add(P, Constants::State_Walking, Constants::D_UpRight, 50, 4,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_UpRight, 50, 42,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_UpRight, 50, 82,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_UpRight, 50, 118,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_UpRight, 50, 155,  40, 40, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State_Walking, Constants::D_DownLeft, 120, 4,  40, 40, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State_Walking, Constants::D_DownLeft, 120, 42,  40, 40, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State_Walking, Constants::D_DownLeft, 120, 82,  40, 40, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State_Walking, Constants::D_DownLeft, 120, 118,  40, 40, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State_Walking, Constants::D_DownLeft, 120, 155,  40, 40, 32, 32, &*peasant, true);

    Animation::add(P, Constants::State_Walking, Constants::D_DownRight, 120, 4,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_DownRight, 120, 42,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_DownRight, 120, 82,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_DownRight, 120, 118,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Walking, Constants::D_DownRight, 120, 155,  40, 40, 32, 32, &*peasant, false);






    Animation::add(P, Constants::State_Idle, Constants::D_Down,15, 4,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Dead, Constants::D_Down,0, 0, 40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Spawning, Constants::D_Down,0, 0, 40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Despawned,Constants::D_Down,0, 0,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Combat, Constants::D_Down,0, 0,  40, 40, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State_Harvesting,Constants::D_Down,0, 0,  40, 40, 32, 32, &*peasant, false);
}

void Animation::setupTownHall(){
    int P = Constants::Unit_TownHall;

    Animation::add(P, Constants::State_Idle, Constants::D_Down,270, 150,  110, 100, 96, 96, &*buildings, false);
    Animation::add(P, Constants::State_Idle, Constants::D_Down,270, 150,  110, 100, 96, 96, &*buildings, false);
    Animation::add(P, Constants::State_Dead, Constants::D_Down,270, 150,  110, 100, 96, 96, &*buildings, false);
    Animation::add(P, Constants::State_Spawning, Constants::D_Down,270, 150,  110, 100, 96, 96, &*buildings, false);
    Animation::add(P, Constants::State_Despawned,Constants::D_Down,270, 150,  110, 100, 96, 96, &*buildings, false);
    Animation::add(P, Constants::State_Combat, Constants::D_Down,270, 150,  110, 100, 96, 96, &*buildings, false);
    Animation::add(P, Constants::State_Harvesting,Constants::D_Down,270, 150,  110, 100, 96, 96, &*buildings, false);





}

sf::Sprite &Animation::getNext(std::shared_ptr<Unit> unit) {
    int state = unit->current_state;
    int iteration = unit->animationIterator;
    int unitId = unit->typeId;
    int direction = unit->direction;

    std::vector<std::shared_ptr<sf::Sprite>> &items = Animation::sprites[unitId][state][direction];

    if(items.size() == 0) {
        std::vector<std::shared_ptr<sf::Sprite>> &items = Animation::sprites[unitId][Constants::State_Idle][Constants::D_Down];
        assert(items.size() > 0);
        std::shared_ptr<sf::Sprite> &sprite = items[iteration % items.size()];
        return *sprite;
    }
    std::shared_ptr<sf::Sprite> &sprite = items[iteration % items.size()];


    return *sprite;


}