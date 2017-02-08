from game import Config
from game.state.GenericState import GenericState
from game.const.State import ID_Harvesting
from game.state.Idle import Idle


class Harvesting(GenericState):
    id = "Harvesting"
    type = ID_Harvesting
    default = Idle

    # Harvesting
    harvest_interval = .5 * Config.FRAME_MULTIPLIER
    harvest_timer = 0
    harvest_iterator = 1

    RETURN = 0
    HARVEST = 1

    harvest_target = None

    def __init__(self, unit, attributes={}):
        super().__init__(unit, attributes, Harvesting)

    def toJSON2(self):
        return {
            'harvest_target': self.harvest_target,
            'harvest_timer': self.harvest_timer,
            'harvest_iterator': self.harvest_iterator
        }

    def stage_goto_harvest(self):
        tiles = self.Map.AdjacentMap.adjacent_walkable(*self.harvest_target, 1)

        if not tiles:
            return
        tile = self.unit.shortest_distance(tiles)

        if not self.has_next_state(Harvesting):
            self.add_next(Harvesting, {
                'harvest_target': self.harvest_target
            }, 1)
            self.unit.move(*tile)

    def stage_harvest(self):
        if self.harvest_timer >= self.harvest_interval:

            tile_data = self.Map.get_tile(*self.harvest_target)

            if self.unit.state.inventory_lumber + \
                    self.unit.state.inventory_gold + \
                    self.unit.state.inventory_oil >= self.unit.inventory_space:
                # Inventory is full, return to base
                recall_building = self.unit.closest_recall_building()
                if not recall_building:
                    # Transition to next state since nothing can be done
                    self.transition()
                    return
                else:
                    self.harvest_iterator = Harvesting.RETURN
                    return

            self.unit.state.inventory_gold += tile_data['gold_yield']
            self.unit.state.inventory_lumber += tile_data['lumber_yield']
            self.unit.state.inventory_oil += tile_data['oil_yield']

            # Todo, adding statistics here becaue its thought to be better for RL????
            self.player.statistics['gold_count'] += tile_data['gold_yield']
            self.player.statistics['lumber_count'] += tile_data['lumber_yield']
            self.player.statistics['oil_count'] += tile_data['oil_yield']

            self.harvest_timer = 0

    def stage_recall(self, recall_building):
        # Must walk
        tiles = self.Map.AdjacentMap.adjacent_walkable(recall_building.state.x + recall_building.dimension,
                                                       recall_building.state.y + recall_building.dimension,
                                                       recall_building.dimension + 1)

        if tiles:
            # Wait for free spot to deliver
            tile = self.unit.shortest_distance(tiles)
            if not self.has_next_state(Harvesting):
                self.add_next(Harvesting, {
                    'harvest_iterator': Harvesting.RETURN,
                    'harvest_target': self.harvest_target
                }, 1)
                self.unit.move(*tile)

    def stage_deliver(self):
        self.unit.player.gold += self.unit.state.inventory_gold
        self.unit.player.lumber += self.unit.state.inventory_lumber
        self.unit.player.oil += self.unit.state.inventory_oil
        self.unit.state.inventory_gold = 0
        self.unit.state.inventory_lumber = 0
        self.unit.state.inventory_oil = 0

    def update(self, tick):
        self.harvest_timer += tick

        if self.harvest_iterator == Harvesting.HARVEST:

            distance = self.unit.distance(*self.harvest_target)
            if distance > 1:
                """
                # Unit cannot reach the harvest tile, It therefore must trigger a Walking state to get to the tile
                """
                self.stage_goto_harvest()
            else:
                """
                # Unit can reach the harvest tile. Start harvesting
                """
                self.stage_harvest()

        elif self.harvest_iterator == Harvesting.RETURN:
            """
            # Find closest recall building (Typically a town-center or lumber mill
            """
            recall_building = self.unit.closest_recall_building()

            if recall_building is None:
                self.transition()
                return

            distance = self.unit.distance(
                recall_building.state.x,
                recall_building.state.y,
                recall_building.dimension
            )

            if distance > 1:
                """
                # Unit cannot reach the recall building, This means that the unit must move towards it
                # Triggers Walking state
                """
                self.stage_recall(recall_building)
            else:
                """
                # Unit are ready for delivering goods to the selected recall building
                # When delivery is done, trigger new harvesting round
                """
                self.stage_deliver()
                self.unit.harvest(*self.harvest_target)
