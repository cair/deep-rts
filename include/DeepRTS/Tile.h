//
// Created by Per-Arne on 24.02.2017.
//

#pragma once

#include <string>
#include "Tile.h"
#include "util/Position.h"


class Unit;
class Tilemap;



class Tile{
private:

	//////////////////////////////////////////////////////////////////////////////////
	///
	/// Properties: Current
	///
	//////////////////////////////////////////////////////////////////////////////////
    Tilemap &tilemap;

    bool harvestable;

	bool walkable;

    float walkModifier;
public:
    [[nodiscard]] float getWalkModifier() const;

private:

    int occupantID = -1;

	int resources;

    /// Tile Name
    std::string name;

    int typeId;

    bool depleted;

    //////////////////////////////////////////////////////////////////////////////////
	///
	/// Properties: Before Depletion
	///
	//////////////////////////////////////////////////////////////////////////////////
	const bool newHarvestable;

	const bool newWalkable;

	const float newWalkModifier;

	const int newTypeId;

    const int newResources;

    const std::string newName;

	//////////////////////////////////////////////////////////////////////////////////
	///
	/// Properties: After Depletion
	///
	//////////////////////////////////////////////////////////////////////////////////
	const bool depletedHarvestable;

	const bool depletedWalkable;

	const float depletedWalkModifier;

	const int depletedTypeId;

    const int depletedResources;

    const std::string depletedName;





public:

	const int id;

	const int x;

	const int y;

	const int width;

	const int height;

	const float lumberYield;

	const float goldYield;

	const float stoneYield;




	/// Constructor
    Tile(
			Tilemap &tilemap,
            int id,
			int x,
			int y,
			int w,
			int h,
            const std::string& newName,
			int newTypeId,
			bool newHarvestable,
			bool newWalkable,
            float newWalkModifier,
			int newResources,
            const std::string& depletedName,
            int depletedTypeId,
			bool depletedHarvestable,
			bool depletedWalkable,
			float depletedWalkmMdifier,
            int depletedResources,
            float lumberYield,
            float goldYield,
            float stoneYield
	);


	//////////////////////////////////////////////////////////////////////////////////
	///
	/// Getters
	///
	//////////////////////////////////////////////////////////////////////////////////


	/// Tile Position
	[[nodiscard]] Position getPosition() const;

	/// Check if tile is Attackable
	bool isAttackable(Unit & unit);

	/// Check if tile is Walkable
	[[nodiscard]] bool isWalkable()const;

	/// Check if tile is Harvestable
	[[nodiscard]] bool isHarvestable()const;

	/// Check if tile is Buildable
	[[nodiscard]] bool isBuildable()const;

    /// Get distance between this tile and another tile
    int distance(Tile &pTile) const;

    /// Get occupant Pointer
    Unit* getOccupant();

    /// Get ID for a occupant on this tile
    [[nodiscard]] int getOccupantID() const;

    /// Check if tile has a occupant
    [[nodiscard]] bool hasOccupant() const;

    /// Get resource count
    [[nodiscard]] int getResources() const;

    [[nodiscard]] bool isDepleted() const;

    [[nodiscard]] int getTypeId() const;

    [[nodiscard]] Tilemap &getTilemap() const;

    [[nodiscard]] const std::string &getName() const;

    [[maybe_unused]] [[nodiscard]] int getDepleteTile() const;

	void triggerOnTileChange();

    //////////////////////////////////////////////////////////////////////////////////
    ///
    /// Setters
    ///
    //////////////////////////////////////////////////////////////////////////////////

    void setOccupant(Unit *unit);
    void setOccupantID(int unitID);
    void setDepleted();
    void setResources(uint16_t resource_count);
    void takeResource(uint8_t n);

    //////////////////////////////////////////////////////////////////////////////////
    ///
    /// Functions
    ///
    //////////////////////////////////////////////////////////////////////////////////

    /// Reset tile
    void reset();








};



