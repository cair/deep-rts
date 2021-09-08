//
// Created by Per-Arne on 23.02.2017.
//

#pragma once
#include <pybind11/pybind11.h>
#include "../../include/DeepRTS/Game.h"
#include "unit/Unit.h"
class PyGame: public Game {
    using Game::Game;

    /*PyGame(std::string map_name): Game(map_name){

    }*/


    const cv::Mat& _render() const override {
        PYBIND11_OVERLOAD(const cv::Mat&, Game, _render, );
    }

    void _caption()const override {
        PYBIND11_OVERLOAD(void, Game, _caption, );
    }

    void _update() override {
        PYBIND11_OVERLOAD(void, Game, _update, );
    }

    void _reset() override {
        PYBIND11_OVERLOAD(void, Game, _reset, );
    }

    void _onUnitCreate(const Unit & unit)const override {
        PYBIND11_OVERLOAD_NAME(void, Game, "_on_unit_create", _onUnitCreate, unit);
    }

    void _onUnitDestroy(const Unit & unit)const override {
        PYBIND11_OVERLOAD_NAME(void, Game, "_on_unit_destroy", _onUnitDestroy, unit);
    }

    void _onTileChange(const Tile & tile)const override {
        PYBIND11_OVERLOAD_NAME(void, Game, "_on_tile_change", _onTileChange, tile);
    }

    void _onEpisodeStart()const override {
        PYBIND11_OVERLOAD_NAME(void, Game, "_on_episode_start", _onEpisodeStart, );
    }

    void _onEpisodeEnd()const override {
        PYBIND11_OVERLOAD_NAME(void, Game, "_on_episode_end", _onEpisodeEnd, );
    }

    void _onResourceGather(const Tile& tile, const Unit& unit)const override {
        PYBIND11_OVERLOAD_NAME(void, Game, "_on_resource_gather", _onResourceGather, tile, unit);
    }

    void _onResourceDepleted(const Tile& tile, const Unit& unit)const override {
        PYBIND11_OVERLOAD_NAME(void, Game, "_on_resource_depleted", _onResourceDepleted, tile, unit);
    }



};
