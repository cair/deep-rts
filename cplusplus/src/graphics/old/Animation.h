//
// Created by Per-Arne on 25.02.2017.
//

#ifndef WARC2SIM_SPRITEMANAGER_H
#define WARC2SIM_SPRITEMANAGER_H


#include <SFML/Graphics/Sprite.hpp>
#include <vector>
#include <map>
#include "../unit/Unit.h"


class Animation
{
public:
    // Unit -> State -> Direction ->sprite[]
    std::map<
            int, std::map<int,
                    std::map<int, std::vector<std::shared_ptr<sf::Sprite>>
                    >>> sprites;
    sf::Sprite & getNext(Unit & unit);

	Animation();
	void add(const int unit, const int state,  const int direction,int x, int y, int w, int h, int wrs, int hrs,  sf::Texture* texture, bool flip);
    void setup();

private:
    std::shared_ptr<sf::Texture> peasant;
    std::shared_ptr<sf::Texture> buildings;
    std::shared_ptr<sf::Texture> archer;
    std::shared_ptr<sf::Texture> footman;

    void setupPeasant();
    void setupTownHall();
    void setupFarm();
    void setupBarracks();
    void setupFootman();
    void setupArcher();
};









#endif //WARC2SIM_SPRITEMANAGER_H
