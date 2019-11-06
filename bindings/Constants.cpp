#include <pybind11/pybind11.h>
namespace py = pybind11;

#include "../src/Constants.h"

void init_Constants(py::module &m) {

    auto constants = m.def_submodule("Constants");

    constants.attr("MAX_PLAYERS") = py::int_(Constants::MAX_PLAYERS);
    constants.attr("MAX_UNITS") = py::int_(Constants::MAX_UNITS);
    constants.attr("action_min") = py::int_(Constants::ACTION_MIN);
    constants.attr("action_max") = py::int_(Constants::ACTION_MAX);


    py::enum_<Constants::Unit>(constants, "Unit", py::arithmetic(), "Unit Constants")
            .value("Peasant", Constants::Unit::Peasant)
            .value("Peon", Constants::Unit::Peon)
            .value("TownHall", Constants::Unit::TownHall)
            .value("Barracks", Constants::Unit::Barracks)
            .value("Footman", Constants::Unit::Footman)
            .value("Farm", Constants::Unit::Farm)
            .value("Archer", Constants::Unit::Archer)
            .value("None", Constants::Unit::None);

    py::enum_<Constants::Race>(constants, "Race", py::arithmetic(), "Race Constants")
            .value("Human", Constants::Race::Human)
            .value("Orc", Constants::Race::Orc);

    py::enum_<Constants::Tile>(constants, "Tile", py::arithmetic(), "Tile Constants")
            .value("Spawn", Constants::Tile::Spawn)
            .value("Grass", Constants::Tile::Grass)
            .value("Wall", Constants::Tile::Wall)
            .value("Lumber", Constants::Tile::Lumber)
            .value("Water", Constants::Tile::Water)
            .value("Gold", Constants::Tile::Gold);

    py::enum_<Constants::State>(constants, "State", py::arithmetic(), "State Constants")
            .value("Idle", Constants::State::Idle)
            .value("Spawning", Constants::State::Spawning)
            .value("Walking", Constants::State::Walking)
            .value("Despawned", Constants::State::Despawned)
            .value("Harvesting", Constants::State::Harvesting)
            .value("Building", Constants::State::Building)
            .value("Combat", Constants::State::Combat)
            .value("Dead", Constants::State::Dead)
            .value("Base", Constants::State::Base);

    py::enum_<Constants::Pathfinding>(constants, "Pathfinding", py::arithmetic(), "Pathfinding Constants")
            .value("Walkable", Constants::Pathfinding::Walkable)
            .value("All", Constants::Pathfinding::All)
            .value("Attackable", Constants::Pathfinding::Attackable)
            .value("Harvestable", Constants::Pathfinding::Harvestable);

    py::enum_<Constants::Direction>(constants, "Direction", py::arithmetic(), "Direction Constants")
            .value("Down", Constants::Direction::Down)
            .value("Up", Constants::Direction::Up)
            .value("Left", Constants::Direction::Left)
            .value("Right", Constants::Direction::Right)
            .value("DownLeft", Constants::Direction::DownLeft)
            .value("DownRight", Constants::Direction::DownRight)
            .value("UpLeft", Constants::Direction::UpLeft)
            .value("UpRight", Constants::Direction::UpRight);

    py::enum_<Constants::Action>(constants, "Action", py::arithmetic(), "Action Constants")
            .value("PreviousUnit", Constants::Action::PreviousUnit)
            .value("NextUnit", Constants::Action::NextUnit)
            .value("MoveLeft", Constants::Action::MoveLeft)
            .value("MoveRight", Constants::Action::MoveRight)
            .value("MoveUp", Constants::Action::MoveUp)
            .value("MoveDown", Constants::Action::MoveDown)
            .value("MoveUpLeft", Constants::Action::MoveUpLeft)
            .value("MoveUpRight", Constants::Action::MoveUpRight)
            .value("MoveDownLeft", Constants::Action::MoveDownLeft)
            .value("MoveDownRight", Constants::Action::MoveDownRight)
            .value("Attack", Constants::Action::Attack)
            .value("Harvest", Constants::Action::Harvest)
            .value("Build0", Constants::Action::Build0)
            .value("Build1", Constants::Action::Build1)
            .value("Build2", Constants::Action::Build2)
            .value("NoAction", Constants::Action::NoAction);
}