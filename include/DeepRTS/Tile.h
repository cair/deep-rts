//
// Created by Per-Arne on 24.02.2017.
//

#pragma once

#include <string>
#include "Tile.h"
#include "util/Position.h"
#include "Constants.h"

namespace DeepRTS {

    class Unit;

    class Tilemap;


/// The tile class represent a single tile in the Tilemap
/// A unit can occupy a tile
/// Resources can occupy a tile
/// A tile can be swimable, walkable, non-walkable and so on.
    class Tile {
    private:


        //////////////////////////////////////////////////////////////////////////////////
        ///
        /// Properties: Before Depletion
        ///
        //////////////////////////////////////////////////////////////////////////////////
        /// Original tile state: is it harvestable?
        const bool newHarvestable;

        /// Original tile state: is it walkable?
        const bool newWalkable;

        /// Original tile walk modifier
        const double newWalkModifier;

        /// Original tile state: the tile type
        const int newTypeId;

        /// Original tile state: resource count
        const int newResources;

        /// Original tile state: the name of the tile
        const std::string newName;

        /// Original damage
        const double newDamageModifier;

        //////////////////////////////////////////////////////////////////////////////////
        ///
        /// Properties: After Depletion
        ///
        //////////////////////////////////////////////////////////////////////////////////
        /// Depleted state: is it harvestable?
        const bool depletedHarvestable;

        /// Depleted state: is it walkable?
        const bool depletedWalkable;

        /// Depleted state: walk modifier
        const double depletedWalkModifier;

        /// Depleted state: type of the tile.
        const int depletedTypeId;

        /// Depleted state: number of resources
        const int depletedResources;

        /// Depleted state: name of the tile
        const std::string depletedName;

        /// Original damage
        const double depletedDamageModifier;

        //////////////////////////////////////////////////////////////////////////////////
        ///
        /// Properties: Current
        ///
        //////////////////////////////////////////////////////////////////////////////////
        /// Reference to the parent tilemap
        Tilemap &tilemap;

        /// Is this tile harvestable?
        bool harvestable;

        /// Is this walkable?
        bool walkable;

        /// WalkModifier of a tile slows/accelerates the walk speed.
        double walkModifier;

        /// Integer that signifies if a unit is occupying the tile
        int occupantID = -1;

        /// How many resources this tile has
        int resources;

        /// Name of the tile (Grass, Water ...etc)
        std::string name;

        /// The type ID of the tile
        int typeId;

        /// If the tile resource is depleted or not.
        bool depleted;

        /// Damage MOdifier
        double damageModifier;

    public:
        /// Get the walk modifier of the specified tile
        /// \return modifier value
        [[nodiscard]] double getWalkModifier() const;

        /// The TileID
        const int id;

        /// The x position of the tile
        const int x;

        /// THe y position of the tile
        const int y;

        /// The width of the tile (usually 1)
        const int width;

        /// The height of the tile (usually 1)
        const int height;

        /// The number of lumber the tile yields on harvesting
        const double lumberYield;

        /// The number of hgold the tile yields on harvesting
        const double goldYield;

        /// The number of stone the tile yields on harvesting.
        const double stoneYield;

        /// A horrible Tile constructor which requires all properties of a tile. Usually you dont want to touch this....
        /// \param tilemap
        /// \param id
        /// \param x
        /// \param y
        /// \param w
        /// \param h
        /// \param newName
        /// \param newTypeId
        /// \param newHarvestable
        /// \param newWalkable
        /// \param newWalkModifier
        /// \param newResources
        /// \param depletedName
        /// \param depletedTypeId
        /// \param depletedHarvestable
        /// \param depletedWalkable
        /// \param depletedWalkmMdifier
        /// \param depletedResources
        /// \param lumberYield
        /// \param goldYield
        /// \param stoneYield
        Tile(
                Tilemap &tilemap,
                int id,
                int x,
                int y,
                int w,
                int h,
                std::string newName,
                int newTypeId,
                bool newHarvestable,
                bool newWalkable,
                double newWalkModifier,
                int newResources,
                std::string depletedName,
                int depletedTypeId,
                bool depletedHarvestable,
                bool depletedWalkable,
                double depletedWalkmMdifier,
                int depletedResources,
                double lumberYield,
                double goldYield,
                double stoneYield,
                double newDamageModifier,
                double depletedDamageModifier
        );


        //////////////////////////////////////////////////////////////////////////////////
        ///
        /// Getters
        ///
        //////////////////////////////////////////////////////////////////////////////////


        /// Retrieve the position of the tile
        /// \return The position
        [[nodiscard]] Position getPosition() const;

        /// Check if there is a entity occupying the tile which is attackable
        /// \param unit attacker unit reference
        /// \return is there a attackable unit here?
        bool isAttackable(Unit &unit);

        /// Check if tile is Walkable
        /// \return is it walkable?
        [[nodiscard]] bool isWalkable() const;

        /// Check if tile is Harvestable
        /// \return is it harvestable?
        [[nodiscard]] bool isHarvestable() const;

        /// Check if tile is Buildable
        /// \return is it buildable?
        [[nodiscard]] bool isBuildable() const;

        /// Get distance between this tile and another tile
        /// \param sourceTile the source tile
        /// \return distance in air
        int distance(Tile &sourceTile) const;

        /// Get occupant Unit pointer
        /// \return Unit* occupant
        Unit *getOccupant();

        /// Get ID for a occupant on this tile
        /// \return id of occupant
        [[nodiscard]] int getOccupantID() const;

        /// Check if tile has a occupant
        /// \return evaluation
        [[nodiscard]] bool hasOccupant() const;

        /// Get resource count
        /// \return resource count
        [[nodiscard]] int getResources() const;

        /// Check if the tile is depleted.
        /// \return isDepleted?
        [[nodiscard]] bool isDepleted() const;

        /// Get the type if this tile
        /// \return TIle ID
        [[nodiscard]] int getTypeId() const;

        /// Get the tilemap reference (tile's parent)
        /// \return
        [[nodiscard]] Tilemap &getTilemap() const;

        /// Get the name of the tile
        /// \return name
        [[nodiscard]] const std::string &getName() const;

        /// Get the tile ID when this tile is depleted.
        /// \return
        [[maybe_unused]] [[nodiscard]] int getDepleteTile() const;

        /// This function is used to trigger a callback which signifies that a tile has changed.
        /// Used to render graphics when required.
        void triggerOnTileChange();

        //////////////////////////////////////////////////////////////////////////////////
        ///
        /// Setters
        ///
        //////////////////////////////////////////////////////////////////////////////////

        /// SEt the occupant using a Pointer to the unit.
        /// \param unit
        void setOccupant(Unit *unit);

        /// Set the occupant id of this tile
        /// \param unitID
        void setOccupantID(int unitID);

        /// Sets the tile to deplted.
        void setDepleted();

        /// Set the resouce count of this tile
        /// \param resource_count
        void setResources(uint16_t resource_count);

        /// Take a n count of resources from this tile.
        /// \param n
        void takeResource(uint8_t n);

        //////////////////////////////////////////////////////////////////////////////////
        ///
        /// Functions
        ///
        //////////////////////////////////////////////////////////////////////////////////

        /// Reset tile
        void reset();


        void update(Unit &unit) const;
    };
}


