//
// Created by Per-Arne on 24.02.2017.
//
#pragma once

#include "unit/Unit.h"
#include "Constants.h"
class Player;

/// The unit manager is used to construct units in the game.
/// The unit manager functions usually takes in a player object as reference, and returns a unit object which is added to the player
/// API is due to change.
class UnitManager {

public:

    /// Construct a unit of type unitType for Player object player
    /// \param unitType The unit type
    /// \param player The player
    /// \return The newly constructed unit
	static Unit constructUnit(Constants::Unit unitType, Player &player);

    /// Update the unit count of a specified unit for player p
    /// \param p the player reference
    /// \param unitType the unit type
    /// \param n the count you wish to add.
	static void updateUnitCount(Player &p, Constants::Unit unitType, int n);

    /// Construct a town hall unit for player
    /// \param player player reference
    /// \return Town-Hall unit
	static Unit constructTownHall(Player &player);

    /// Construct a peasant unit for player
    /// \param player player reference
    /// \return Peasant unit
	static Unit constructPeasant(Player &player);

    /// Construct a farm unit for player
    /// \param player player reference
    /// \return Farm unit
	static Unit constructFarm(Player &player);

    /// Construct a barracks unit for player
    /// \param player player reference
    /// \return Barracks unit
	static Unit constructBarracks(Player &player);

    /// Construct a footman unit for player
    /// \param player player reference
    /// \return Footman Unit
    static Unit constructFootman(Player &player);

    /// Create a archer unit for player
    /// \param player player reference
    /// \return Archer unit
    static Unit constructArcher(Player &player);
};


