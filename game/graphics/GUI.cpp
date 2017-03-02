//
// Created by Per-Arne on 24.02.2017.
//
/*
 *
2
down vote
accepted
I've run into a similar issue myself and only after starting the application outside of the CLion IDE you'll actually get the informative error, which is that you're missing a DLL.

If you use the dynamic libraries, you'll have to copy the DLLs into the working directory or next to your application. Also don't forget to copy over the OpenAL DLL that ships with SFML if you ever want to use the audio module.

I really hope JetBrains can report the missing DLL error better in their IDE.
 */

#include "GUI.h"
#include "../Game.h"
#include "Animation.h"
#include <SFML/Graphics.hpp>



GUI::GUI(Game &game) :
        game(game),
        window(sf::VideoMode(800, 800), "WarC2Sim++", sf::Style::Titlebar | sf::Style::Close /*| sf::Style::Resize*/){


    player = game.players[0];


    this->createView();
    this->createFrame();


    this->font.loadFromFile("./data/fonts/arial.ttf");


}

void GUI::createView(){
    const Tilemap &tilemap = game.map;
    this->fullView = sf::View(sf::FloatRect(0, 0,
                                            tilemap.MAP_WIDTH * tilemap.TILE_WIDTH,
                                            tilemap.MAP_HEIGHT * tilemap.TILE_HEIGHT));
    this->fullView.setViewport(sf::FloatRect(0, 0, 1, 1));
    this->fullView.zoom(1);


    this->minimapView = sf::View(sf::FloatRect(0, 0,
                                               tilemap.MAP_WIDTH * tilemap.TILE_WIDTH,
                                               tilemap.MAP_HEIGHT * tilemap.TILE_HEIGHT));
    this->minimapView.setViewport(sf::FloatRect(.01, .25, .20, .22));

    this->povView = sf::View(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    this->povView.setViewport(sf::FloatRect(0, 0, 1, 1));
    this->povView.zoom(.9);


    this->currentView = &this->povView;
}


void GUI::createFrame(){

    this->gameFrameTexture.loadFromFile("data/textures/game_frame.png");

    this->gameFrame.setTexture(this->gameFrameTexture);
    /*std::cout << this->gameFrameTexture.getSize().x << std::endl;
    this->gameFrame.setScale(sf::Vector2f((float)this->gameFrameTexture.getSize().x / window.getSize().x,
                                          (float)this->gameFrameTexture.getSize().y / window.getSize().y));
                                          */

}

void GUI::caption() {
    std::stringstream ss;
    ss << "WarC2Sim++ [FPS=" << game.currentFPS << ",UPS=" << game.currentUPS << "]";
    std::string s = ss.str();
    window.setTitle(s);

}

sf::Vector2f GUI::getCameraOffset() {
    sf::Vector2f viewCenter = currentView->getCenter();
    sf::Vector2f halfExtents = currentView->getSize() / 2.0f;
    sf::Vector2f translation = viewCenter - halfExtents;
    return translation;
}

int GUI::mouseClick(int mX, int mY)
{
    int x = mX / this->game.map.TILE_WIDTH;
    int y = mY / this->game.map.TILE_HEIGHT;
    int cols = this->game.map.MAP_WIDTH;

    int idx = cols*y + x;

    return idx;

}

void GUI::handleEvents(){
    // Process events
    sf::Event event;
    while (window.pollEvent(event))
    {
        // Close window: exit
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                // get the current mouse position in the window
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                int idx = this->mouseClick(worldPos.x, worldPos.y);
                Tile &t = this->game.map.tiles[idx];
                this->leftClick(t);

            } else  if (event.mouseButton.button == sf::Mouse::Right)
            {
                // get the current mouse position in the window
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                int idx = this->mouseClick(worldPos.x, worldPos.y);
                Tile &t = this->game.map.tiles[idx];
                this->rightClick(t);

            }
        }


        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Left)
                this->panLeft = true;
            else if(event.key.code == sf::Keyboard::Right)
                this->panRight = true;
            else if(event.key.code == sf::Keyboard::Up)
                this->panUp = true;
            else if(event.key.code == sf::Keyboard::Down)
                this->panDown = true;


                // Change View
            else if(event.key.code == sf::Keyboard::Q) {
                this->currentView = &povView;
            }
            else if(event.key.code == sf::Keyboard::W) {
                this->currentView = &fullView;
            }
            else if(event.key.code == sf::Keyboard::R) {
                this->toggleFrame = !this->toggleFrame;
            }

                // Change FPS and UPS
            else if(event.key.code == sf::Keyboard::Comma) {
                game.fps -= 2;
                game.setFPS(game.fps);
            }
            else if(event.key.code == sf::Keyboard::Period) {
                game.fps += 2;
                game.setFPS(game.fps);
            }
                //  UPS
            else if(event.key.code == sf::Keyboard::N) {
                game.ups -= 2;
                game.setUPS(game.ups);
            }
            else if(event.key.code == sf::Keyboard::M) {
                game.ups += 2;
                game.setUPS(game.ups);
            }


            else if(event.key.code == sf::Keyboard::G) {
                renderGUI = !renderGUI;
                if(!renderGUI){
                    showNoGuiMessage();
                }
            }

            else if(event.key.code == sf::Keyboard::H) {
                renderGUI = !renderGUI;
                if(!renderGUI){
                    showGridLines = !showGridLines;
                }
            }

            else if(event.key.code == sf::Keyboard::F) {
                game.setUPS(10);
                game.setFPS(60);
            }




                // Build actions
            else if(event.key.code == sf::Keyboard::Num1) {
                if(player->targetedUnit){
                    player->targetedUnit->build(0);
                }
            }
            else if(event.key.code == sf::Keyboard::Num2) {
                if(player->targetedUnit){
                    player->targetedUnit->build(1);
                }
            }
            else if(event.key.code == sf::Keyboard::Num3) {
                if(player->targetedUnit){
                    player->targetedUnit->build(2);
                }
            }
        }
        else if(event.type == sf::Event::KeyReleased)
        {
            if(event.key.code == sf::Keyboard::Left)
                this->panLeft = false;
            else if(event.key.code == sf::Keyboard::Right)
                this->panRight = false;
            else if(event.key.code == sf::Keyboard::Up)
                this->panUp = false;
            else if(event.key.code == sf::Keyboard::Down)
                this->panDown = false;
        }
    }
}

void GUI::render() {


    handleEvents();


    if(!renderGUI)
        return;

    // Draw things here
    window.setView(*this->currentView);
    this->drawTiles();
    this->drawUnits();

    window.setView(this->fullView);

    this->drawStats();
    this->drawSelected();  // Text
    this->drawStatistics();
    this->drawScoreBoard();


    /*if(this->toggleFrame){
        window.setView(this->fullView);
        window.draw(this->gameFrame);

        // TODO heavy
        window.setView(this->minimapView);
        this->drawTiles();
        this->drawUnits();
    }*/


    if(!renderGUI){
        showNoGuiMessage();
        return;
    }

    // Update the window
    window.display();

}

void GUI::update(){

    sf::Vector2f viewSize = currentView->getSize();
    sf::Vector2f offset = getCameraOffset();

    if(this->panLeft && offset.x > 0)
        this->povView.move(sf::Vector2f(this->velocity * -1, 0));
    if(this->panRight && offset.x < (game.map.MAP_WIDTH * game.map.TILE_WIDTH) -  viewSize.x)
        this->povView.move(sf::Vector2f(this->velocity, 0));
    if(this->panUp && offset.y > 0)
        this->povView.move(sf::Vector2f(0, this->velocity * -1));
    if(this->panDown && offset.y < (game.map.MAP_HEIGHT * game.map.TILE_HEIGHT) -  viewSize.y)
        this->povView.move(sf::Vector2f(0, this->velocity));

}

void GUI::drawStats(){
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Yellow);

    text.setString("Lumber: " + std::to_string(player->getLumber()));
    text.setPosition(10,10);
    window.draw(text);

    text.setString("Gold: " + std::to_string(player->getGold()));
    text.setPosition(125,10);
    window.draw(text);

    text.setString("Oil: " + std::to_string(player->getOil()));
    text.setPosition(225,10);
    window.draw(text);

    text.setString("Food: " + std::to_string(player->getFoodConsumption()) + "/" + std::to_string(player->getFood()));
    text.setPosition(300,10);
    window.draw(text);

    text.setString("Units: " + std::to_string(player->getUnitCount()));
    text.setPosition(380,10);
    window.draw(text);

    text.setString("Seconds: " + std::to_string(game.getSeconds()));
    text.setPosition(470,10);
    window.draw(text);

    text.setString("Frames: " + std::to_string(game.getFrames()));
    text.setPosition(640,10);
    window.draw(text);

    text.setString("Score: " + std::to_string(player->getScore()));
    text.setPosition(810,10);
    window.draw(text);

}

void GUI::drawStatistics(){
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Yellow);

    text.setString("CurrentFPS: " + std::to_string(game.currentFPS));
    text.setPosition(790,50);
    window.draw(text);

    text.setString("CurrentUPS: " + std::to_string(game.currentUPS));
    text.setPosition(790,75);
    window.draw(text);

    text.setString("FPS: " + std::to_string(game.fps));
    text.setPosition(790,100);
    window.draw(text);

    text.setString("UPS: " + std::to_string(game.ups));
    text.setPosition(790,125);
    window.draw(text);
}


void GUI::drawSelected(){
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Yellow);

    if(selectedTile)
    {
        text.setString("Type: " + selectedTile->name + " - (" + std::to_string(selectedTile->x) + "," + std::to_string(selectedTile->y) + ")");
        text.setPosition(10,860);
        window.draw(text);


        text.setString((selectedTile->harvestable) ? "Harvestable: Yes" : "Harvestable: No");
        text.setPosition(10,880);
        window.draw(text);

        text.setString((selectedTile->walkable) ? "Walkable: Yes" : "Walkable: No");
        text.setPosition(10,900);
        window.draw(text);

        text.setString("Resources: " + std::to_string(selectedTile->resources));
        text.setPosition(10,920);
        window.draw(text);
    }



    if (player->targetedUnit) {
        text.setCharacterSize(32);
        std::shared_ptr<Unit> unit = player->targetedUnit;

        text.setString(unit->name + " (" +
                       std::to_string(unit->id ) +
                       ")" + " - " +
                       unit->state->name + " - (" +
                       std::to_string(unit->tile->x) + "," +
                       std::to_string(unit->tile->y) + ") - (" +
                       std::to_string(unit->player_.getId()) + ")"
        );
        text.setPosition(320,830);
        window.draw(text);

        text.setCharacterSize(20);

        text.setString("Lumber: " + std::to_string(unit->lumberCarry));
        text.setPosition(270,870);
        window.draw(text);

        text.setString("Gold: " + std::to_string(unit->goldCarry));
        text.setPosition(270,900);
        window.draw(text);

        text.setString("Oil: " + std::to_string(unit->oilCarry));
        text.setPosition(270,930);
        window.draw(text);

        text.setString("Health: " + std::to_string(unit->health) + "/" + std::to_string(unit->health_max));
        text.setPosition(385,870);
        window.draw(text);

        text.setString("Damage: " + std::to_string(unit->damageMin) + " - " + std::to_string(unit->damageMax));
        text.setPosition(385, 900);
        window.draw(text);

        text.setString("Armor: " + std::to_string(unit->armor));
        text.setPosition(385,930);
        window.draw(text);

        text.setString((unit->groundUnit) ? "Ground: Yes" : "Ground: No");
        text.setPosition(550,870);
        window.draw(text);

        text.setString((unit->waterUnit) ? "Water: Yes" : "Water: No");
        text.setPosition(550,900);
        window.draw(text);

        text.setString("Speed: " + std::to_string(unit->speed));
        text.setPosition(550,930);
        window.draw(text);

    }

}

void GUI::drawTiles(){



    for(const Tile& tile : game.map.tiles){


        if(showGridLines){
            sf::RectangleShape rectangle;
            rectangle.setSize(sf::Vector2f(32, 32));
            rectangle.setOutlineColor(sf::Color::Red);
            rectangle.setOutlineThickness(1);
            rectangle.setPosition(tile.getPixelPosition());
            window.draw(rectangle);
        }


        window.draw(tile.vertices, 4, sf::Quads, &game.map.tileset);




    }



}

void GUI::drawUnits() {

    for(auto&p : game.players)
    {
        for(auto&u: p->units){
            if(u->tile) {
                u->animationTimer++;
                if(u->animationTimer >= u->animationInterval){
                    u->animationIterator += 1;
                    u->animationTimer = 0;

                }

                //u->testSprite->setColor(u->player_.playerColor);
                sf::Sprite &sprite = Animation::getInstance().getNext(u);
                sprite.setPosition(u->worldPosition);
                window.draw(sprite);
            }
        }
    }

}

void GUI::leftClick(Tile &tile) {
    this->selectedTile = &tile;

    if(tile.occupant) {
        player->targetedUnit = tile.occupant;
    }else {
        player->targetedUnit = NULL;
    }
}

void GUI::rightClick(Tile &tile) {
    if(player->targetedUnit) {
        player->targetedUnit->rightClick(tile);
        this->selectedTile = &tile;
    }
}

void GUI::drawScoreBoard() {
    sf::Text text;
    text.setFont(font);



    int offsetY = 0;
    text.setCharacterSize(18);
    for (Player *p : game.players) {
        text.setString(p->name_ + ": " + std::to_string(p->getScore()));
        text.setFillColor(p->playerColor);
        text.setPosition(10, 40 + offsetY);
        offsetY += 25;

        window.draw(text);
    }


}

void GUI::showNoGuiMessage() {


    window.clear();


    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Yellow);
    text.setString("GUI is deactivated! Press \"G\" to activate.");
    sf::Vector2u size = window.getSize();
    text.setPosition((size.x / 2) - (text.getLocalBounds().width / 4) ,size.y / 2);
    window.draw(text);

    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Yellow);
    text.setString("Hotkeys:\nG: toggle gui\n,: Decrease FPS\n.: Increase FPS\nQ: Pov View\n W: World View\nF: GameMode (10UPS/60FPS)\nH: Gridlines");
    text.setPosition((size.x / 2), (size.y / 2) + 50);
    window.draw(text);

    window.display();
}
