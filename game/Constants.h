//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_CONSTANTS_H
#define WARC2SIM_CONSTANTS_H

namespace Constants{
    const int Unit_Peasant = 0;
    const int Unit_Peon = 1;
    const int Unit_TownHall = 2;

    const int Player_Race_Human = 0;
    const int Player_Race_Orc = 1;

    const int Tile_Spawn = 16;
    const int Tile_Grass = 271;


    const int State_Idle = 0;
    const int State_Spawning = 1;
    const int State_Walking = 2;
    const int State_Despawned = 3;
    const int State_Harvesting = 4;
    const int State_Building = 5;
    const int State_Combat = 6;
    const int State_Dead = 7;

    const int Pathfinding_Walkable = 1;
    const int Pathfinding_All = 2;
}






#endif //WARC2SIM_CONSTANTS_H
