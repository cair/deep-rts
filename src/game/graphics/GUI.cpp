#include <thread>
#include "GUI.h"
#include "../Game.h"




GUI::GUI(Game &game) :
        game(game),
        map(game.map.tiles),
        window(sf::VideoMode(800, 800), "DeepRTS v1.3", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize),
        player(&game.players[pIterator]){

    setupView();
    setupFrame();
    setupFont();
    setupPlot();

	if (Config::getInstance().getAudioEnabled()) {
		setupAudio();
	}








}

void GUI::setupPlot() {

    plot.setPosition(sf::Vector2f(13, 240));
    plot.setSize(sf::Vector2f(180, 205));
    plot.setTitle("DeepRTS Agent Performance");
    plot.setFont("./data/fonts/arial.ttf");
    plot.setXLabel("Tick");
    plot.setYLabel("Score");
    plot.setBackgroundColor(sf::Color(rand()%255, rand()%255, rand()%255));
    plot.setTitleColor(sf::Color::Black);

    for(auto &player : game.players) {

        sf::plot::Curve &curve = plot.createCurve(
                player.name_,
                sf::Color(std::get<0>(player.playerColor), std::get<1>(player.playerColor), std::get<2>(player.playerColor))
        );
        curve.setLimit(20000);
        curve.setLabel(player.name_);
        curve.addValue(0);

    }
}

void GUI::setupFont() {
    if(font.loadFromFile("./data/fonts/arial.ttf")) {
        // Loaded font

    } else {
        assert(false && "Could not load font");
    }
}

void GUI::setupAudio(){
    auto *music = new sf::Music();
    if (music->openFromFile(".//data//audio//song_1.ogg"))
    {
		music->setVolume(Config::getInstance().getAudioVolume());
        music->setLoop(true);
        music->play();
    }

}

void GUI::setupView(){
    const Tilemap &tilemap = game.map;
    this->fullView = sf::View(sf::FloatRect(0, 0,
                                            static_cast<float_t>(800),
                                            static_cast<float_t>(800)));
    this->fullView.setViewport(sf::FloatRect(0, 0, 1, 1));
    this->fullView.zoom(1);

    this->frameView = sf::View(sf::FloatRect(0, 0,
                                             static_cast<float_t>(800),
                                             static_cast<float_t>(800)));
    this->frameView.setViewport(sf::FloatRect(0.22, 0.00, 0.77, 0.78));
    this->frameView.zoom(1.30);

    this->povView = sf::View(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    this->povView.setViewport(sf::FloatRect(0, 0, 1, 1));
    this->povView.zoom(.9);


    this->currentView = &this->fullView;
}


void GUI::setupFrame(){
    // tilemap.MAP_WIDTH*tilemap.MAP_HEIGHT
    this->gameFrameTexture.loadFromFile("data/textures/game_frame.png");
    this->gameFrame.setTexture(this->gameFrameTexture);
    this->gameFrame.setScale(800.0/2560,800.0/1440);
}

void GUI::reset() {
    plot.reset();
}

sf::Vector2f GUI::getCameraOffset() {
    sf::Vector2f viewCenter = currentView->getCenter();
    sf::Vector2f halfExtents = currentView->getSize() / 2.0f;
    sf::Vector2f translation = viewCenter - halfExtents;
    return translation;
}

int GUI::mouseClick(int mX, int mY)
{
    size_t x = mX / this->game.map.TILE_WIDTH;
    size_t y = mY / this->game.map.TILE_HEIGHT;
    size_t cols = this->game.map.MAP_WIDTH;

    size_t idx = cols*y + x;

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
                if (idx > game.map.tiles.size()) return;
                Tile &t = this->game.map.tiles[idx];
                this->leftClick(t);

            } else  if (event.mouseButton.button == sf::Mouse::Right)
            {
                // get the current mouse position in the window
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                int idx = this->mouseClick(worldPos.x, worldPos.y);
                if (idx > game.map.tiles.size()) return;
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


            else if (event.key.code == sf::Keyboard::J) {
                // Previous player (Focus)
                pIterator -= 1;
                player = &game.players[pIterator % game.players.size()];
            }

            else if (event.key.code == sf::Keyboard::K) {
				// Next Player (Focus)
                pIterator += 1;
                player = &game.players[pIterator % game.players.size()];
            }




                // Build actions
            else if(event.key.code == sf::Keyboard::Num1) {
                if(player->getTargetedUnit()){
                    player->getTargetedUnit()->build(0);
                }
            }
            else if(event.key.code == sf::Keyboard::Num2) {
                if(player->getTargetedUnit()){
                    player->getTargetedUnit()->build(1);
                }
            }
            else if(event.key.code == sf::Keyboard::Num3) {
                if(player->getTargetedUnit()){
                    player->getTargetedUnit()->build(2);
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

    // Handle input events
    handleEvents();

    // If nogui is toggled (Black screen)
    if(!renderGUI){
        showNoGuiMessage();
        return;
    }

    // Set to frame view
    window.setView(this->frameView);
    this->drawTiles();
    this->drawUnits();
    this->drawPlayerSelectedUnit();


    // Full view (0,0, w,h)
    window.setView(this->fullView);
    window.draw(this->gameFrame);

    this->drawStats();
    this->drawSelected();  // Text
    this->drawStatistics();
    this->drawScoreBoard();
    this->drawActionDistribution();



    // Update plot
    /*for(auto &p : game.players){
        sf::plot::Curve &curve = plot.getCurve(p.name_);
        curve.addValue(p.getScore());
    }
    plot.prepare();
    window.draw(plot);*/

    window.setView(this->frameView);
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

#if SFML_VERSION_MAJOR == 2 and SFML_VERSION_MINOR >= 4
    text.setFillColor(sf::Color::Yellow);
#else
    text.setColor(sf::Color::Yellow);
#endif

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
    text.setPosition(580,10);
    window.draw(text);

    text.setString("Score: " + std::to_string(player->getScore()));
    text.setPosition(700,10);
    window.draw(text);

}

void GUI::drawStatistics(){
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
#if SFML_VERSION_MAJOR == 2 and SFML_VERSION_MINOR >= 4
    text.setFillColor(sf::Color::Yellow);
#else
    text.setColor(sf::Color::Yellow);
#endif
    int offsetX = -780;

    text.setString("CurrentFPS: " + std::to_string(game.currentFPS));
    text.setPosition(790 + offsetX,50);
    window.draw(text);

    text.setString("CurrentUPS: " + std::to_string(game.currentUPS));
    text.setPosition(790 + offsetX,75);
    window.draw(text);

    text.setString("FPS: " + std::to_string(game.fps));
    text.setPosition(790 + offsetX,100);
    window.draw(text);

    text.setString("UPS: " + std::to_string(game.ups));
    text.setPosition(790 + offsetX,125);
    window.draw(text);

    text.setString("Selected: " + player->name_);
    text.setPosition(790 + offsetX, 150);
    window.draw(text);

    text.setString("Game: " + std::to_string(game.getGameCount()));
    text.setPosition(790 + offsetX, 175);
    window.draw(text);
}

void GUI::drawActionDistribution() {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(15);
#if SFML_VERSION_MAJOR == 2 and SFML_VERSION_MINOR >= 4
    text.setFillColor(sf::Color::Yellow);
#else
    text.setColor(sf::Color::Yellow);
#endif

    for (int i = 0; i < sizeof(Constants::actionNames) / sizeof(Constants::actionNames[0]); i++) {
        text.setString(Constants::actionNames[i] + ": " + std::to_string(player->actionStatistics[i]));
        text.setPosition(10, 380 + (20 * i));
        window.draw(text);
    }



}

void GUI::drawSelected(){
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
#if SFML_VERSION_MAJOR == 2 and SFML_VERSION_MINOR >= 4
    text.setFillColor(sf::Color::Yellow);
#else
    text.setColor(sf::Color::Yellow);
#endif

    int offsetY = -100;

    if(selectedTile)
    {
        text.setString(selectedTile->name + " - (" + std::to_string(selectedTile->x) + "," + std::to_string(selectedTile->y) + ")");
        text.setPosition(680, 140 + offsetY);
        window.draw(text);


        text.setString((selectedTile->harvestable) ? "Harvestable: Yes" : "Harvestable: No");
        text.setPosition(800,980 + offsetY);
        window.draw(text);

        text.setString((selectedTile->walkable) ? "Walkable: Yes" : "Walkable: No");
        text.setPosition(800,1000 + offsetY);
        window.draw(text);

        text.setString("Resources: " + std::to_string(selectedTile->getResources()));
        text.setPosition(800,1020 + offsetY);
        window.draw(text);
    }



    if (player->getTargetedUnit()){
        int tOffsetX = 0;
        int tOffsetY = 690;
        text.setCharacterSize(26);
        Unit *unit = player->getTargetedUnit();
        Tile *tile = (unit->tile) ? unit->tile : game.getMap().getTile(0, 0);
        text.setString(unit->name + " (" +
                       std::to_string(unit->id ) +
                       ")" + " - " +
                       unit->state->name + " - (" +
                       std::to_string(tile->x) + "," +
                       std::to_string(tile->y) + ") - (" +
                       std::to_string(unit->player_->id_) + ")"
        );
        text.setPosition(270 + tOffsetX, 0 + tOffsetY);
        window.draw(text);

        text.setCharacterSize(18);

        text.setString("Lumber: " + std::to_string(unit->lumberCarry));
        text.setPosition(270 + tOffsetX, 30 + tOffsetY);
        window.draw(text);

        text.setString("Gold: " + std::to_string(unit->goldCarry));
        text.setPosition(270 + tOffsetX, 50 + tOffsetY);
        window.draw(text);

        text.setString("Oil: " + std::to_string(unit->oilCarry));
        text.setPosition(270 + tOffsetX, 70 + tOffsetY);
        window.draw(text);

        text.setString("Health: " + std::to_string(unit->health) + "/" + std::to_string(unit->health_max));
        text.setPosition(375 + tOffsetX, 30 + tOffsetY);
        window.draw(text);

        text.setString("Damage: " + std::to_string(unit->damageMin) + " - " + std::to_string(unit->damageMax));
        text.setPosition(375 + tOffsetX, 50 + tOffsetY);
        window.draw(text);

        text.setString("Armor: " + std::to_string(unit->armor));
        text.setPosition(375 + tOffsetX, 70 + tOffsetY);
        window.draw(text);

        text.setString((unit->groundUnit) ? "Ground: Yes" : "Ground: No");
        text.setPosition(540 + tOffsetX, 30 + tOffsetY);
        window.draw(text);

        text.setString((unit->waterUnit) ? "Water: Yes" : "Water: No");
        text.setPosition(540 + tOffsetX, 50 + tOffsetY);
        window.draw(text);

        text.setString("Speed: " + std::to_string(unit->speed));
        text.setPosition(540 + tOffsetX, 70 + tOffsetY);
        window.draw(text);



    }

}

unsigned char * GUI::capture(){
    sf::Texture txt;
    txt.create(window.getSize().x, window.getSize().y);
    txt.update(window);
    captured_image = txt.copyToImage();
    return const_cast<unsigned char *>(captured_image.getPixelsPtr());
}

void GUI::drawTiles(){

    if(Config::getInstance().mechanicFOW){
        auto &gMap = game.getMap();
        sf::RectangleShape rectangle;
        rectangle.setSize(sf::Vector2f(gMap.MAP_WIDTH * gMap.TILE_WIDTH, gMap.TILE_HEIGHT * gMap.MAP_HEIGHT));
        rectangle.setFillColor(sf::Color::Black);
        rectangle.setPosition(0, 0);
        window.draw(rectangle);


		
	
        for(auto &u_idx : player->unitIndexes) {
			auto u = game.getUnit(u_idx);
            auto idxs = u.getVisionTileIDs();


            for(auto &idx : idxs) {
                auto &gTile = map.gTiles[idx];
                if(gTile.dataTile.depleted) {
                    window.draw(gTile.deplete_verticles, 4, sf::Quads, &map.tileset);
                } else {
                    window.draw(gTile.tile_verticles, 4, sf::Quads, &map.tileset);
                }
            }
        }

    } else {
        for(GraphicTile& gTile : map.gTiles){

            if (showGridLines) {
                sf::RectangleShape rectangle;
                rectangle.setSize(sf::Vector2f(32, 32));
                rectangle.setOutlineThickness(1);
                rectangle.setPosition(gTile.getPixelPosition());

                window.draw(rectangle);
            }

            if(gTile.dataTile.depleted) {
                window.draw(gTile.deplete_verticles, 4, sf::Quads, &map.tileset);
            } else {
                window.draw(gTile.tile_verticles, 4, sf::Quads, &map.tileset);
            }

        }
    }









}

void GUI::drawUnits() {

    for (auto&u : game.units) {
        if (u.tile) {
            u.animationTimer++;
            if (u.animationTimer >= u.animationInterval) {
                u.animationIterator += 1;
                u.animationTimer = 0;

            }

            //u.testSprite->setColor(u.player_->playerColor);
            sf::Sprite &sprite = animation.getNext(u);
            sprite.setPosition(map.gTiles[u.tile->id].getPixelPosition());

            auto &colorData = u.player_->playerColor;
            auto color = sf::Color(std::get<0>(colorData), std::get<1>(colorData), std::get<2>(colorData));
            sprite.setColor(color);
            window.draw(sprite);
        }
    }
}

void GUI::leftClick(Tile &tile) {
    this->selectedTile = &tile;

    if(tile.getOccupant()) {
        player->targetedUnitID = tile.getOccupant()->id;
    }else {
        player->targetedUnitID = -1;
    }
}

void GUI::rightClick(Tile &tile) {
    if(player->getTargetedUnit()) {
        player->getTargetedUnit()->rightClick(tile);
        this->selectedTile = &tile;
    }
}

void GUI::drawScoreBoard() {
    sf::Text text;
    text.setFont(font);

    int offsetY = 0;
    text.setCharacterSize(14);
    for (Player & p : game.players) {
        text.setString(p.name_ + ": " + std::to_string(p.getScore()) + " | APM: " + std::to_string(p.apm) + " | AQueue: " + std::to_string(p.getQueueSize()));
        auto &color = p.playerColor;
#if SFML_VERSION_MAJOR == 2 and SFML_VERSION_MINOR >= 4
        text.setFillColor(sf::Color(std::get<0>(color), std::get<1>(color), std::get<2>(color)));
#else
        text.setColor(sf::Color(std::get<0>(color), std::get<1>(color), std::get<2>(color)));
#endif
        text.setPosition(10, 720 - offsetY);
        offsetY += 15;

        window.draw(text);
    }
}

void GUI::drawPlayerSelectedUnit() {
    for (Player & p : game.players) {
        // Draw selected unit
        if (p.getTargetedUnit()) {
            Unit *targetedUnit = p.getTargetedUnit();
            sf::RectangleShape rectangle(sf::Vector2f((targetedUnit->width * 32) + 6, (targetedUnit->height * 32) + 6));
            rectangle.setFillColor(sf::Color::Transparent);
            auto &color = p.playerColor;
            rectangle.setOutlineColor(sf::Color(std::get<0>(color), std::get<1>(color), std::get<2>(color)));
            rectangle.setOutlineThickness(2);
            rectangle.setPosition(sf::Vector2f((targetedUnit->worldPosition.x * 32) - 2, (targetedUnit->worldPosition.y * 32) - 2));

            window.draw(rectangle);
        }
    }
}

void GUI::showNoGuiMessage() {


    window.clear();


    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
#if SFML_VERSION_MAJOR == 2 and SFML_VERSION_MINOR >= 4
    text.setFillColor(sf::Color::Yellow);
#else
    text.setColor(sf::Color::Yellow);
#endif
    text.setString("GUI is deactivated! Press \"G\" to activate.");
    sf::Vector2u size = window.getSize();
    text.setPosition((size.x / 2) - (text.getLocalBounds().width / 4) ,size.y / 2);
    window.draw(text);

    text.setCharacterSize(16);
#if SFML_VERSION_MAJOR == 2 and SFML_VERSION_MINOR >= 4
    text.setFillColor(sf::Color::Yellow);
#else
    text.setColor(sf::Color::Yellow);
#endif
    text.setString("Hotkeys:\nG: toggle gui\n,: Decrease FPS\n.: Increase FPS\nQ: Pov View\n W: World View\nR:Toggle Frame\nF: GameMode (10UPS/60FPS)\nH: Gridlines\nJ: Previous Player\nK Next Player");
    text.setPosition((size.x / 2), (size.y / 2) + 50);
    window.draw(text);

    window.display();
}
