//
// Created by per on 9/5/21.
//
#include "gui/Blend2DGUI.h"
#include "Game.h"
#include <opencv2/highgui.hpp>
#include "Constants.h"

const cv::Mat& Blend2DGUI::render() const {
    return renderData;
}

void Blend2DGUI::onTileChange(const Tile &tile) {

    auto& tileTexture = tileset.at(tile.getTypeId());
    int x_tile = (tile.x * tileSize);
    int y_tile = (tile.y * tileSize);

    if(tile.hasOccupant()){
        auto& unit = game.units.at(tile.getOccupantID());

        /// Draw unit
        if(unit.tile->id == tile.id){
            /// We only want to draw on the first tile. For example, town halls

            /// Draw background
            Blend2DWrapper::blit(x_tile, y_tile, ctx, tileTexture);


            auto& unitTexture = units.at(unit.player_.getId()).at(unit.typeId).get(
                    unit.animationCounter,
                    static_cast<Constants::Direction>(unit.direction),
                    unit.state->id
            );

            int x_unit = (tile.x * tileSize);
            int y_unit = (tile.y * tileSize);
            Blend2DWrapper::blit(x_unit, y_unit, ctx, unitTexture);

        }

    }else{
        /// Redraw tile
        Blend2DWrapper::blit(x_tile, y_tile, ctx, tileTexture);
    }

}

void Blend2DGUI::renderMap(){

    for(int i = 0; i < game.tilemap.tiles.size(); i++) {
        auto x = i % game.getWidth();
        auto y = i / game.getWidth();    // where "/" is an integer division
        int x_start = (x * tileSize);
        int y_start = (y * tileSize);
        auto tileID = game.tilemap.tiles[i].getTypeId();
        auto& tile = tileset.at(tileID);
        Blend2DWrapper::blit(x_start, y_start, ctx, tile);
    }
}

Blend2DGUI::Blend2DGUI(Game &game) : BaseGUI(game)

, canvas(
        int(tileSize * game.getWidth()),
        int(tileSize * game.getHeight()),
        BL_FORMAT_PRGB32)
, ctx(canvas)
, tileset(tileSize, tileSize, "assets/textures/tiles.png")
{
    blImageGetData(&canvas, &imageData); // get ptr to image data
    renderData = cv::Mat(canvas.height(), canvas.width(), CV_8UC4, imageData.pixelData); // create cv proxy


    std::vector<std::tuple<int, int, int>> masks = {
        {0, 0, 255},  
        {255, 0, 0},
        {0, 255, 0},
        {255, 255, 0},
        {0, 255, 255},
        {255, 0, 255},  
        {255, 255, 255},
        {128, 255, 128},
    };

    for(int i = 0; i < game.players.capacity(); i++){

        auto& mask = masks.at(i);
        units.push_back({
                {Constants::Unit::Peasant, Blend2DPeasant(mask, tileSize, 1, 1)},
                //    {Constants::Unit::Peon, Blend2DPeon()},
                {Constants::Unit::TownHall, Blend2DTownHall(mask, tileSize, 3, 3)},
                {Constants::Unit::Barracks, Blend2DBarracks(mask, tileSize, 3, 3)},
                {Constants::Unit::Footman, Blend2DFootman(mask, tileSize, 1, 1)},
                {Constants::Unit::Farm, Blend2DFarm(mask, tileSize, 1, 1)},
                {Constants::Unit::Archer, Blend2DArcher(mask, tileSize, 1, 1)},
                // add more here
        });

    }



    renderMap();
}