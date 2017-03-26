from game.loaders.MapLoader import MapLoader
from game.util import ArrayUtil
from game import const


class AdjacentMap:
    loaded = False

    _map = {}

    _edges_wood = []
    _edges_gold = []

    @staticmethod
    def generate(dim=3):

        assert MapLoader.width and MapLoader.height

        for x in range(MapLoader.height):
            for y in range(MapLoader.width):
                for d in range(0, dim):
                    key = (x, y, d)
                    adjacent_tiles = ArrayUtil.neighbors(*key)

                    AdjacentMap._map[key] = adjacent_tiles

        AdjacentMap.loaded = True

    @staticmethod
    def _calculate_edges():
        pass

    @staticmethod
    def adjacent(x, y, dim):
        return AdjacentMap._map[(x, y, dim)]

    @staticmethod
    def adjacent_walkable(game, x, y, dim):
        potential_tiles = AdjacentMap.adjacent(x, y, dim)

        tiles = []
        for x, y in potential_tiles:
            is_grass = MapLoader.tiles[x][y] == const.Map.ID_GRASS
            is_no_unit = game.data['unit'][x, y] == const.Unit.ID_NONE
            if is_grass and is_no_unit:
                tiles.append((x, y))


        return tiles

    @staticmethod
    def adjacent_harvestable(unit):
        potential_tiles = AdjacentMap.adjacent(
            unit.state.x + unit.dimension,
            unit.state.y + unit.dimension,
            unit.dimension + 1)

        tiles = [(x, y) for x, y in potential_tiles if MapLoader.tiles[x][y]in [const.Map.ID_WOOD, const.Map.ID_GOLD]]

        return tiles

    @staticmethod
    def adjacent_attackable(unit):
        potential_tiles = AdjacentMap.adjacent(
            unit.state.x + unit.dimension,
            unit.state.y + unit.dimension,
            unit.dimension + 1)

        tiles = [(x, y) for x, y in potential_tiles if unit.game.data['unit_pid'][x, y] not in [unit.player.id, 0]]

        return tiles
