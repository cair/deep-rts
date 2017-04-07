//
// Created by Per-Arne on 25.02.2017.
//

#include <SFML/Graphics/Texture.hpp>

#include <memory>
#include "Animation.h"
#include "../Constants.h"

Animation::Animation() {
	setup();
};

void Animation::add(const int unit, const int state, const int direction, int x, int y, int w, int h, int wrs, int hrs, sf::Texture* texture, bool flip) {

    std::shared_ptr<sf::Sprite> sprite = std::shared_ptr<sf::Sprite>(new sf::Sprite());
    sprite->setTexture(*texture);
    if(flip) {
        //wrs *= -1; // TODO this messes up placement
    }

    sprite->setTextureRect(sf::IntRect(x, y, w, h));
    sprite->setScale((double)wrs/w,(double)hrs/h);
    Animation::sprites[unit][state][direction].push_back(sprite);



}

void Animation::setup() {
    peasant = std::shared_ptr<sf::Texture>(new sf::Texture());
    peasant->loadFromFile("data/textures/human/peasant.png");

    archer = std::shared_ptr<sf::Texture>(new sf::Texture());
    archer->loadFromFile("data/textures/human/archer.png");

    footman = std::shared_ptr<sf::Texture>(new sf::Texture());
    footman->loadFromFile("data/textures/human/footman.png");

    buildings = std::shared_ptr<sf::Texture>(new sf::Texture());
    buildings->loadFromFile("data/textures/human/buildings.png");


    setupPeasant();
    setupTownHall();
    setupFarm();
    setupBarracks();
    setupFootman();
    setupArcher();


}

void Animation::setupPeasant(){
    int P = Constants::Unit::Peasant;
    assert(peasant);


    ///
    /// Walking
    ///
    Animation::add(P, Constants::State::Walking, Constants::Direction::Up, 18, 8,  25, 23, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Up, 18, 46,  23, 28, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Up, 18, 86,  23, 26, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Up, 18, 122,  23, 30, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Up, 18, 159,  24, 27, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State::Walking, Constants::Direction::Down, 166, 7,  24, 26, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Down, 169, 45,  22, 26, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Down, 167, 85,  26, 27, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Down, 168, 121,  23, 26, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Down, 167, 158,  24, 27, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State::Walking, Constants::Direction::Left, 97, 4,  14, 31, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Left, 91, 42,  24, 30, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Left, 96, 82,  16, 31, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Left, 91, 118,  23, 30, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Left, 95, 155,  20, 30, 32, 32, &*peasant, true);

    Animation::add(P, Constants::State::Walking, Constants::Direction::Right, 97, 4,  14, 31, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Right, 91, 42,  24, 30, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Right, 96, 82,  16, 31, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Right, 91, 118,  23, 30, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Right, 95, 155,  20, 30, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State::Walking, Constants::Direction::UpRight, 56, 6,  22, 26, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpRight, 55, 44,  26, 30, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpRight, 56, 84,  24, 29, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpRight, 56, 119,  23, 29, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpRight, 57, 156,  21, 28, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State::Walking, Constants::Direction::UpLeft, 56, 6,  22, 26, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpLeft, 55, 44,  26, 30, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpLeft, 56, 84,  24, 29, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpLeft, 56, 119,  23, 29, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpLeft, 57, 156,  21, 28, 32, 32, &*peasant, true);

    Animation::add(P, Constants::State::Walking, Constants::Direction::DownRight, 128, 3,  21, 31, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownRight, 128, 41,  20, 27, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownRight, 130, 82,  19, 26, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownRight, 126, 119,  26, 29, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownRight, 126, 156,  26, 28, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State::Walking, Constants::Direction::DownLeft, 128, 3,  21, 31, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownLeft, 128, 41,  20, 27, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownLeft, 130, 82,  19, 26, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownLeft, 126, 119,  26, 29, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownLeft, 126, 156,  26, 28, 32, 32, &*peasant, true);

    ///
    /// Harvesting
    ///
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,168, 196,  24, 25, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,160, 228,  28, 25, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,168, 274,  23, 24, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,174, 317,  22, 37, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,173, 367,  22, 24, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Up,19, 190,  31, 32, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Up,19, 230,  30, 27, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Up,19, 260,  23, 41, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Up,19, 304,  21, 41, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Up,18, 356,  24, 36, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownRight,129, 196,  21, 27, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownRight,122, 231,  25, 26, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownRight,129, 273,  19, 29, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownRight,133, 319,  37, 27, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownRight,134, 366,  32, 28, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownLeft,129, 196,  21, 27, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownLeft,122, 231,  25, 26, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownLeft,129, 273,  19, 29, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownLeft,133, 319,  37, 27, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownLeft,134, 366,  32, 28, 32, 32, &*peasant, true);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpRight,56, 196,  27, 25, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpRight,56, 231,  21, 25, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpRight,56, 270,  30, 30, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpRight,56, 309,  31, 35, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpRight,56, 362,  32, 30, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpLeft,56, 196,  27, 25, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpLeft,56, 231,  21, 25, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpLeft,56, 270,  30, 30, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpLeft,56, 309,  31, 35, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpLeft,56, 362,  32, 30, 32, 32, &*peasant, true);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Right,96, 194,  19, 27, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Right,94, 228,  18, 27, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Right,93, 268,  27, 32, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Right,94, 314,  36, 29, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Right,94, 364,  35, 27, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Left,96, 194,  19, 27, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Left,94, 228,  18, 27, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Left,93, 268,  27, 32, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Left,94, 314,  36, 29, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Left,94, 364,  35, 27, 32, 32, &*peasant, true);

    
    ///
    /// Combat
    ///
    Animation::add(P, Constants::State::Combat,Constants::Direction::Down,168, 196,  24, 25, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Down,160, 228,  28, 25, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Down,168, 274,  23, 24, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Down,174, 317,  22, 37, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Down,173, 367,  22, 24, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State::Combat,Constants::Direction::Up,19, 190,  31, 32, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Up,19, 230,  30, 27, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Up,19, 260,  23, 41, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Up,19, 304,  21, 41, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Up,18, 356,  24, 36, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State::Combat,Constants::Direction::DownRight,129, 196,  21, 27, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownRight,122, 231,  25, 26, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownRight,129, 273,  19, 29, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownRight,133, 319,  37, 27, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownRight,134, 366,  32, 28, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State::Combat,Constants::Direction::DownLeft,129, 196,  21, 27, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownLeft,122, 231,  25, 26, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownLeft,129, 273,  19, 29, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownLeft,133, 319,  37, 27, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownLeft,134, 366,  32, 28, 32, 32, &*peasant, true);

    Animation::add(P, Constants::State::Combat,Constants::Direction::UpRight,56, 196,  27, 25, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpRight,56, 231,  21, 25, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpRight,56, 270,  30, 30, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpRight,56, 309,  31, 35, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpRight,56, 362,  32, 30, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State::Combat,Constants::Direction::UpLeft,56, 196,  27, 25, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpLeft,56, 231,  21, 25, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpLeft,56, 270,  30, 30, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpLeft,56, 309,  31, 35, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpLeft,56, 362,  32, 30, 32, 32, &*peasant, true);

    Animation::add(P, Constants::State::Combat,Constants::Direction::Right,96, 194,  19, 27, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Right,94, 228,  18, 27, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Right,93, 268,  27, 32, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Right,94, 314,  36, 29, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Right,94, 364,  35, 27, 32, 32, &*peasant, false);

    Animation::add(P, Constants::State::Combat,Constants::Direction::Left,96, 194,  19, 27, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Left,94, 228,  18, 27, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Left,93, 268,  27, 32, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Left,94, 314,  36, 29, 32, 32, &*peasant, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Left,94, 364,  35, 27, 32, 32, &*peasant, true);

    Animation::add(P, Constants::State::Idle, Constants::Direction::Down,166, 7,  24, 26, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Dead, Constants::Direction::Down,166, 7,  24, 26, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Spawning, Constants::Direction::Down,166, 7,  24, 26, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Despawned, Constants::Direction::Down,166, 7,  24, 26, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat, Constants::Direction::Down,166, 7,  24, 26, 32, 32, &*peasant, false);
    Animation::add(P, Constants::State::Combat, Constants::Direction::Down,0, 0,  40, 40, 32, 32, &*peasant, false);

}

void Animation::setupTownHall(){
    int P = Constants::Unit::TownHall;

    Animation::add(P, Constants::State::Idle, Constants::Direction::Down,270, 150,  110, 100, 96, 96, &*buildings, false);
    Animation::add(P, Constants::State::Idle, Constants::Direction::Up,270, 15,  110, 100, 96, 96, &*buildings, false);

	Animation::add(P, Constants::State::Building, Constants::Direction::Down, 270, 150, 110, 100, 96, 96, &*buildings, false);
	Animation::add(P, Constants::State::Building, Constants::Direction::Up, 270, 15, 110, 100, 96, 96, &*buildings, false);

    Animation::add(P, Constants::State::Dead, Constants::Direction::Down,270, 150,  110, 100, 96, 96, &*buildings, false);
    Animation::add(P, Constants::State::Spawning, Constants::Direction::Down,270, 150,  110, 100, 96, 96, &*buildings, false);
    Animation::add(P, Constants::State::Despawned,Constants::Direction::Down,270, 150,  110, 100, 96, 96, &*buildings, false);
    Animation::add(P, Constants::State::Combat, Constants::Direction::Down,270, 150,  110, 100, 96, 96, &*buildings, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,270, 150,  110, 100, 96, 96, &*buildings, false);
}

void Animation::setupBarracks(){
    int P = Constants::Unit::Barracks;
    assert(buildings);

    Animation::add(P, Constants::State::Idle, Constants::Direction::Down,306, 568,  96, 81, 96, 96, &*buildings, false);
    Animation::add(P, Constants::State::Idle, Constants::Direction::Up,306, 457,  96, 95, 96, 96, &*buildings, false);

    Animation::add(P, Constants::State::Building, Constants::Direction::Down, 306, 568,  96, 81, 96, 96, &*buildings, false);
    Animation::add(P, Constants::State::Building, Constants::Direction::Up, 306, 457,  96, 95, 96, 96, &*buildings, false);

    Animation::add(P, Constants::State::Dead, Constants::Direction::Down,306, 568,  96, 81, 96, 96, &*buildings, false);
    Animation::add(P, Constants::State::Spawning, Constants::Direction::Down,306, 568,  96, 81, 96, 96, &*buildings, false);
    Animation::add(P, Constants::State::Despawned,Constants::Direction::Down,306, 568,  96, 81, 96, 96, &*buildings, false);
    Animation::add(P, Constants::State::Combat, Constants::Direction::Down,306, 568,  96, 81, 96, 96, &*buildings, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,306, 568,  96, 81, 96, 96, &*buildings, false);
}

void Animation::setupFarm() {
    int P = Constants::Unit::Farm;
    assert(buildings);

    Animation::add(P, Constants::State::Idle, Constants::Direction::Down, 397, 72,  63, 59, 64, 64, &*buildings, false);
    Animation::add(P, Constants::State::Idle, Constants::Direction::Up, 397, 3,  63, 59, 64, 64, &*buildings, false);

    Animation::add(P, Constants::State::Building, Constants::Direction::Down, 397, 72,  63, 59, 64, 64, &*buildings, false);
    Animation::add(P, Constants::State::Building, Constants::Direction::Up, 397, 3,  63, 59, 64, 64, &*buildings, false);

    Animation::add(P, Constants::State::Dead, Constants::Direction::Down,397, 72,  63, 59, 64, 64, &*buildings, false);
    Animation::add(P, Constants::State::Spawning, Constants::Direction::Down,397, 72,  63, 59, 64, 64, &*buildings, false);
    Animation::add(P, Constants::State::Despawned,Constants::Direction::Down,397, 72,  63, 59, 64, 64, &*buildings, false);
    Animation::add(P, Constants::State::Combat, Constants::Direction::Down,397, 72,  63, 59, 64, 64, &*buildings, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,397, 72,  63, 59, 64, 64, &*buildings, false);
}

void Animation::setupFootman(){
    int P = Constants::Unit::Footman;
    assert(footman);

    ///
    /// Walking
    ///
    Animation::add(P, Constants::State::Walking, Constants::Direction::Up, 18, 8,  25, 23, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Up, 18, 46,  23, 28, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Up, 18, 86,  23, 26, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Up, 18, 122,  23, 30, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Up, 18, 159,  24, 27, 32, 32, &*footman, false);

    Animation::add(P, Constants::State::Walking, Constants::Direction::Down, 166, 7,  24, 26, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Down, 169, 45,  22, 26, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Down, 167, 85,  26, 27, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Down, 168, 121,  23, 26, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Down, 167, 158,  24, 27, 32, 32, &*footman, false);

    Animation::add(P, Constants::State::Walking, Constants::Direction::Left, 97, 4,  14, 31, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Left, 91, 42,  24, 30, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Left, 96, 82,  16, 31, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Left, 91, 118,  23, 30, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Left, 95, 155,  20, 30, 32, 32, &*footman, true);

    Animation::add(P, Constants::State::Walking, Constants::Direction::Right, 97, 4,  14, 31, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Right, 91, 42,  24, 30, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Right, 96, 82,  16, 31, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Right, 91, 118,  23, 30, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Right, 95, 155,  20, 30, 32, 32, &*footman, false);

    Animation::add(P, Constants::State::Walking, Constants::Direction::UpRight, 56, 6,  22, 26, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpRight, 55, 44,  26, 30, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpRight, 56, 84,  24, 29, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpRight, 56, 119,  23, 29, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpRight, 57, 156,  21, 28, 32, 32, &*footman, false);

    Animation::add(P, Constants::State::Walking, Constants::Direction::UpLeft, 56, 6,  22, 26, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpLeft, 55, 44,  26, 30, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpLeft, 56, 84,  24, 29, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpLeft, 56, 119,  23, 29, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpLeft, 57, 156,  21, 28, 32, 32, &*footman, true);

    Animation::add(P, Constants::State::Walking, Constants::Direction::DownRight, 128, 3,  21, 31, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownRight, 128, 41,  20, 27, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownRight, 130, 82,  19, 26, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownRight, 126, 119,  26, 29, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownRight, 126, 156,  26, 28, 32, 32, &*footman, false);

    Animation::add(P, Constants::State::Walking, Constants::Direction::DownLeft, 128, 3,  21, 31, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownLeft, 128, 41,  20, 27, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownLeft, 130, 82,  19, 26, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownLeft, 126, 119,  26, 29, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownLeft, 126, 156,  26, 28, 32, 32, &*footman, true);

    ///
    /// Harvesting
    ///
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,168, 196,  24, 25, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,160, 228,  28, 25, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,168, 274,  23, 24, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,174, 317,  22, 37, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,173, 367,  22, 24, 32, 32, &*footman, false);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Up,19, 190,  31, 32, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Up,19, 230,  30, 27, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Up,19, 260,  23, 41, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Up,19, 304,  21, 41, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Up,18, 356,  24, 36, 32, 32, &*footman, false);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownRight,129, 196,  21, 27, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownRight,122, 231,  25, 26, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownRight,129, 273,  19, 29, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownRight,133, 319,  37, 27, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownRight,134, 366,  32, 28, 32, 32, &*footman, false);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownLeft,129, 196,  21, 27, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownLeft,122, 231,  25, 26, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownLeft,129, 273,  19, 29, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownLeft,133, 319,  37, 27, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownLeft,134, 366,  32, 28, 32, 32, &*footman, true);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpRight,56, 196,  27, 25, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpRight,56, 231,  21, 25, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpRight,56, 270,  30, 30, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpRight,56, 309,  31, 35, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpRight,56, 362,  32, 30, 32, 32, &*footman, false);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpLeft,56, 196,  27, 25, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpLeft,56, 231,  21, 25, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpLeft,56, 270,  30, 30, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpLeft,56, 309,  31, 35, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpLeft,56, 362,  32, 30, 32, 32, &*footman, true);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Right,96, 194,  19, 27, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Right,94, 228,  18, 27, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Right,93, 268,  27, 32, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Right,94, 314,  36, 29, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Right,94, 364,  35, 27, 32, 32, &*footman, false);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Left,96, 194,  19, 27, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Left,94, 228,  18, 27, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Left,93, 268,  27, 32, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Left,94, 314,  36, 29, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Left,94, 364,  35, 27, 32, 32, &*footman, true);


    ///
    /// Combat
    ///
    Animation::add(P, Constants::State::Combat,Constants::Direction::Down,168, 196,  24, 25, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Down,160, 228,  28, 25, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Down,168, 274,  23, 24, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Down,174, 317,  22, 37, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Down,173, 367,  22, 24, 32, 32, &*footman, false);

    Animation::add(P, Constants::State::Combat,Constants::Direction::Up,19, 190,  31, 32, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Up,19, 230,  30, 27, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Up,19, 260,  23, 41, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Up,19, 304,  21, 41, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Up,18, 356,  24, 36, 32, 32, &*footman, false);

    Animation::add(P, Constants::State::Combat,Constants::Direction::DownRight,129, 196,  21, 27, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownRight,122, 231,  25, 26, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownRight,129, 273,  19, 29, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownRight,133, 319,  37, 27, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownRight,134, 366,  32, 28, 32, 32, &*footman, false);

    Animation::add(P, Constants::State::Combat,Constants::Direction::DownLeft,129, 196,  21, 27, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownLeft,122, 231,  25, 26, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownLeft,129, 273,  19, 29, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownLeft,133, 319,  37, 27, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownLeft,134, 366,  32, 28, 32, 32, &*footman, true);

    Animation::add(P, Constants::State::Combat,Constants::Direction::UpRight,56, 196,  27, 25, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpRight,56, 231,  21, 25, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpRight,56, 270,  30, 30, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpRight,56, 309,  31, 35, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpRight,56, 362,  32, 30, 32, 32, &*footman, false);

    Animation::add(P, Constants::State::Combat,Constants::Direction::UpLeft,56, 196,  27, 25, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpLeft,56, 231,  21, 25, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpLeft,56, 270,  30, 30, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpLeft,56, 309,  31, 35, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpLeft,56, 362,  32, 30, 32, 32, &*footman, true);

    Animation::add(P, Constants::State::Combat,Constants::Direction::Right,96, 194,  19, 27, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Right,94, 228,  18, 27, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Right,93, 268,  27, 32, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Right,94, 314,  36, 29, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Right,94, 364,  35, 27, 32, 32, &*footman, false);

    Animation::add(P, Constants::State::Combat,Constants::Direction::Left,96, 194,  19, 27, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Left,94, 228,  18, 27, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Left,93, 268,  27, 32, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Left,94, 314,  36, 29, 32, 32, &*footman, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Left,94, 364,  35, 27, 32, 32, &*footman, true);

    Animation::add(P, Constants::State::Idle, Constants::Direction::Down,166, 7,  24, 26, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Dead, Constants::Direction::Down,166, 7,  24, 26, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Spawning, Constants::Direction::Down,166, 7,  24, 26, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Despawned, Constants::Direction::Down,166, 7,  24, 26, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat, Constants::Direction::Down,166, 7,  24, 26, 32, 32, &*footman, false);
    Animation::add(P, Constants::State::Combat, Constants::Direction::Down,0, 0,  40, 40, 32, 32, &*footman, false);
}

void Animation::setupArcher() {
    int P = Constants::Unit::Archer;
    assert(archer);

    ///
    /// Walking
    ///
    Animation::add(P, Constants::State::Walking, Constants::Direction::Up, 18, 8,  25, 23, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Up, 18, 46,  23, 28, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Up, 18, 86,  23, 26, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Up, 18, 122,  23, 30, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Up, 18, 159,  24, 27, 32, 32, &*archer, false);

    Animation::add(P, Constants::State::Walking, Constants::Direction::Down, 166, 7,  24, 26, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Down, 169, 45,  22, 26, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Down, 167, 85,  26, 27, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Down, 168, 121,  23, 26, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Down, 167, 158,  24, 27, 32, 32, &*archer, false);

    Animation::add(P, Constants::State::Walking, Constants::Direction::Left, 97, 4,  14, 31, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Left, 91, 42,  24, 30, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Left, 96, 82,  16, 31, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Left, 91, 118,  23, 30, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Left, 95, 155,  20, 30, 32, 32, &*archer, true);

    Animation::add(P, Constants::State::Walking, Constants::Direction::Right, 97, 4,  14, 31, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Right, 91, 42,  24, 30, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Right, 96, 82,  16, 31, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Right, 91, 118,  23, 30, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::Right, 95, 155,  20, 30, 32, 32, &*archer, false);

    Animation::add(P, Constants::State::Walking, Constants::Direction::UpRight, 56, 6,  22, 26, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpRight, 55, 44,  26, 30, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpRight, 56, 84,  24, 29, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpRight, 56, 119,  23, 29, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpRight, 57, 156,  21, 28, 32, 32, &*archer, false);

    Animation::add(P, Constants::State::Walking, Constants::Direction::UpLeft, 56, 6,  22, 26, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpLeft, 55, 44,  26, 30, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpLeft, 56, 84,  24, 29, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpLeft, 56, 119,  23, 29, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::UpLeft, 57, 156,  21, 28, 32, 32, &*archer, true);

    Animation::add(P, Constants::State::Walking, Constants::Direction::DownRight, 128, 3,  21, 31, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownRight, 128, 41,  20, 27, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownRight, 130, 82,  19, 26, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownRight, 126, 119,  26, 29, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownRight, 126, 156,  26, 28, 32, 32, &*archer, false);

    Animation::add(P, Constants::State::Walking, Constants::Direction::DownLeft, 128, 3,  21, 31, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownLeft, 128, 41,  20, 27, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownLeft, 130, 82,  19, 26, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownLeft, 126, 119,  26, 29, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Walking, Constants::Direction::DownLeft, 126, 156,  26, 28, 32, 32, &*archer, true);

    ///
    /// Harvesting
    ///
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,168, 196,  24, 25, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,160, 228,  28, 25, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,168, 274,  23, 24, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,174, 317,  22, 37, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Down,173, 367,  22, 24, 32, 32, &*archer, false);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Up,19, 190,  31, 32, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Up,19, 230,  30, 27, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Up,19, 260,  23, 41, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Up,19, 304,  21, 41, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Up,18, 356,  24, 36, 32, 32, &*archer, false);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownRight,129, 196,  21, 27, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownRight,122, 231,  25, 26, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownRight,129, 273,  19, 29, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownRight,133, 319,  37, 27, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownRight,134, 366,  32, 28, 32, 32, &*archer, false);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownLeft,129, 196,  21, 27, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownLeft,122, 231,  25, 26, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownLeft,129, 273,  19, 29, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownLeft,133, 319,  37, 27, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::DownLeft,134, 366,  32, 28, 32, 32, &*archer, true);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpRight,56, 196,  27, 25, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpRight,56, 231,  21, 25, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpRight,56, 270,  30, 30, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpRight,56, 309,  31, 35, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpRight,56, 362,  32, 30, 32, 32, &*archer, false);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpLeft,56, 196,  27, 25, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpLeft,56, 231,  21, 25, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpLeft,56, 270,  30, 30, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpLeft,56, 309,  31, 35, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::UpLeft,56, 362,  32, 30, 32, 32, &*archer, true);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Right,96, 194,  19, 27, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Right,94, 228,  18, 27, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Right,93, 268,  27, 32, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Right,94, 314,  36, 29, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Right,94, 364,  35, 27, 32, 32, &*archer, false);

    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Left,96, 194,  19, 27, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Left,94, 228,  18, 27, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Left,93, 268,  27, 32, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Left,94, 314,  36, 29, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Harvesting,Constants::Direction::Left,94, 364,  35, 27, 32, 32, &*archer, true);


    ///
    /// Combat
    ///
    Animation::add(P, Constants::State::Combat,Constants::Direction::Down,168, 196,  24, 25, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Down,160, 228,  28, 25, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Down,168, 274,  23, 24, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Down,174, 317,  22, 37, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Down,173, 367,  22, 24, 32, 32, &*archer, false);

    Animation::add(P, Constants::State::Combat,Constants::Direction::Up,19, 190,  31, 32, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Up,19, 230,  30, 27, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Up,19, 260,  23, 41, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Up,19, 304,  21, 41, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Up,18, 356,  24, 36, 32, 32, &*archer, false);

    Animation::add(P, Constants::State::Combat,Constants::Direction::DownRight,129, 196,  21, 27, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownRight,122, 231,  25, 26, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownRight,129, 273,  19, 29, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownRight,133, 319,  37, 27, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownRight,134, 366,  32, 28, 32, 32, &*archer, false);

    Animation::add(P, Constants::State::Combat,Constants::Direction::DownLeft,129, 196,  21, 27, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownLeft,122, 231,  25, 26, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownLeft,129, 273,  19, 29, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownLeft,133, 319,  37, 27, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::DownLeft,134, 366,  32, 28, 32, 32, &*archer, true);

    Animation::add(P, Constants::State::Combat,Constants::Direction::UpRight,56, 196,  27, 25, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpRight,56, 231,  21, 25, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpRight,56, 270,  30, 30, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpRight,56, 309,  31, 35, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpRight,56, 362,  32, 30, 32, 32, &*archer, false);

    Animation::add(P, Constants::State::Combat,Constants::Direction::UpLeft,56, 196,  27, 25, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpLeft,56, 231,  21, 25, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpLeft,56, 270,  30, 30, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpLeft,56, 309,  31, 35, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::UpLeft,56, 362,  32, 30, 32, 32, &*archer, true);

    Animation::add(P, Constants::State::Combat,Constants::Direction::Right,96, 194,  19, 27, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Right,94, 228,  18, 27, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Right,93, 268,  27, 32, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Right,94, 314,  36, 29, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Right,94, 364,  35, 27, 32, 32, &*archer, false);

    Animation::add(P, Constants::State::Combat,Constants::Direction::Left,96, 194,  19, 27, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Left,94, 228,  18, 27, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Left,93, 268,  27, 32, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Left,94, 314,  36, 29, 32, 32, &*archer, true);
    Animation::add(P, Constants::State::Combat,Constants::Direction::Left,94, 364,  35, 27, 32, 32, &*archer, true);

    Animation::add(P, Constants::State::Idle, Constants::Direction::Down,166, 7,  24, 26, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Dead, Constants::Direction::Down,166, 7,  24, 26, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Spawning, Constants::Direction::Down,166, 7,  24, 26, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Despawned, Constants::Direction::Down,166, 7,  24, 26, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat, Constants::Direction::Down,166, 7,  24, 26, 32, 32, &*archer, false);
    Animation::add(P, Constants::State::Combat, Constants::Direction::Down,0, 0,  40, 40, 32, 32, &*archer, false);
}


sf::Sprite &Animation::getNext(Unit & unit) {
    int state = unit.state->id;
    int iteration = unit.animationIterator;
    int unitId = unit.typeId;
    int direction = unit.direction;

    std::vector<std::shared_ptr<sf::Sprite>> &items = Animation::sprites[unitId][state][direction];

    if(items.size() == 0) {
        std::vector<std::shared_ptr<sf::Sprite>> &items = Animation::sprites[unitId][Constants::State::Idle][Constants::Direction::Down];
        assert(items.size() > 0);
        std::shared_ptr<sf::Sprite> &sprite = items[iteration % items.size()];
        return *sprite;
    }
    std::shared_ptr<sf::Sprite> &sprite = items[iteration % items.size()];


    return *sprite;


}