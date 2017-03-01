//
// Created by Per-Arne on 25.02.2017.
//

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include "Animation.h"
#include "../Constants.h"


void Animation::add(const int unit, const int state, const int direction, int x, int y, int w, int h, sf::Texture* texture) {

    std::shared_ptr<sf::Sprite> sprite = std::shared_ptr<sf::Sprite>(new sf::Sprite());
    sprite->setTexture(*texture);
    sprite->setTextureRect(sf::IntRect(x, y, w, h));

    Animation::sprites[unit][state][direction].push_back(sprite);



}

void Animation::setup() {
    peasant = std::shared_ptr<sf::Texture>(new sf::Texture());
    peasant->loadFromFile("data/textures/human/peasant.png");

    setupPeasant();


}

void Animation::setupPeasant(){
    int P = Constants::Unit_Peasant;


    Animation::add(P, Constants::State_Walking, Constants::D_Down, 0, 0,  40, 40, &*peasant);
    Animation::add(P, Constants::State_Walking, Constants::D_Down, 55, 90, 40, 40, &*peasant);





    Animation::add(P, Constants::State_Idle, Constants::D_Down,0, 0,  40, 40, &*peasant);
    Animation::add(P, Constants::State_Idle, Constants::D_Down,55, 90,40, 40, &*peasant);
    Animation::add(P, Constants::State_Idle, Constants::D_Down,33, 90, 40, 40, &*peasant);
    Animation::add(P, Constants::State_Idle, Constants::D_Down,61, 90, 40, 40, &*peasant);
    Animation::add(P, Constants::State_Idle, Constants::D_Down,11, 90, 40, 40, &*peasant);
    Animation::add(P, Constants::State_Idle, Constants::D_Down,12, 90, 40, 40, &*peasant);

    Animation::add(P, Constants::State_Dead, Constants::D_Down,0, 0, 40, 40, &*peasant);
    Animation::add(P, Constants::State_Spawning, Constants::D_Down,0, 0, 40, 40, &*peasant);
    Animation::add(P, Constants::State_Despawned,Constants::D_Down,0, 0,  40, 40, &*peasant);
    Animation::add(P, Constants::State_Combat, Constants::D_Down,0, 0,  40, 40, &*peasant);
    Animation::add(P, Constants::State_Harvesting,Constants::D_Down,0, 0,  40, 40, &*peasant);
}

sf::Sprite &Animation::getNext(std::shared_ptr<Unit> unit) {
    int state = unit->state->id;
    int iteration = unit->animationIterator;
    int unitId = unit->typeId;
    int direction = unit->direction;

    std::vector<std::shared_ptr<sf::Sprite>> &items = Animation::sprites[unitId][state][direction];

    if(items.size() == 0) {
        std::vector<std::shared_ptr<sf::Sprite>> &items = Animation::sprites[unitId][state][Constants::D_Down];
        assert(items.size() > 0);
        std::shared_ptr<sf::Sprite> &sprite = items[iteration % items.size()];
        return *sprite;
    }
    std::shared_ptr<sf::Sprite> &sprite = items[iteration % items.size()];


    return *sprite;


}