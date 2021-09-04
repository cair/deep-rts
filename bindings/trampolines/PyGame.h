//
// Created by Per-Arne on 23.02.2017.
//

#pragma once

#include "../../src/Game.h"
#include "../../src/unit/Unit.h"
class PyGame: public Game {
    using Game::Game;

    /*PyGame(std::string map_name): Game(map_name){

    }*/


    void _render() override {
        PYBIND11_OVERLOAD(void, Game, _render, );
    }

    void _caption() override {
        PYBIND11_OVERLOAD(void, Game, _caption, );
    }

    void _update() override {
        PYBIND11_OVERLOAD(void, Game, _update, );
    }

    void _reset() override {
        PYBIND11_OVERLOAD(void, Game, _reset, );
    }

    void _onUnitCreate(Unit & unit) override {
        PYBIND11_OVERLOAD_NAME(void, Game, "_on_unit_create", _onUnitCreate, unit);
    }

    void _onUnitDestroy(Unit & unit) override {
        PYBIND11_OVERLOAD_NAME(void, Game, "_on_unit_destroy", _onUnitDestroy, unit);
    }

    void _onTileChange(Tile & tile) override {
        PYBIND11_OVERLOAD_NAME(void, Game, "_on_tile_change", _onTileChange, tile);
    }

    void _onEpisodeStart() override {
        PYBIND11_OVERLOAD_NAME(void, Game, "_on_episode_start", _onEpisodeStart, );
    }

    void _onEpisodeEnd() override {
        PYBIND11_OVERLOAD_NAME(void, Game, "_on_episode_end", _onEpisodeEnd, );
    }

    void _onResourceGather(Tile& tile, Unit& unit) override {
        PYBIND11_OVERLOAD_NAME(void, Game, "_on_resource_gather", _onResourceGather, tile, unit);
    }

    void _onResourceDepleted(Tile& tile, Unit& unit) override {
        PYBIND11_OVERLOAD_NAME(void, Game, "_on_resource_depleted", _onResourceDepleted, tile, unit);
    }



};
