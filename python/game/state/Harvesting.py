from game import Config
from game import const
from game.state.GenericState import GenericState




class Harvesting(GenericState):
    id = "Harvesting"
    type = const.State.ID_Harvesting


    # Harvesting
    harvest_interval = .5 * Config.FRAME_MULTIPLIER
    harvest_timer = 0
    harvest_iterator = 1

    RETURN = 0
    HARVEST = 1

    harvest_target = None

    def __init__(self, unit):
        super().__init__(unit)
        self.data = unit.d['s'][Harvesting.type]

    def init(self):
        self.data['harvest_timer'] = 0

    def stage_goto_harvest(self):
        tiles = self.game.AdjacentMap.adjacent_walkable(self.game, *self.data['harvest_target'], 1)

        if not tiles:
            return
        tile = self.unit.shortest_distance(tiles)

        if not self.has_next_state(const.State.ID_Harvesting):
            self.add_next(const.State.ID_Harvesting, {
                'harvest_target': self.data['harvest_target']
            }, 1)
            self.unit.move(*tile)

    def stage_harvest(self):
        if self.data['harvest_timer'] >= self.harvest_interval:

            tile_data = self.Map.get_tile(*self.data['harvest_target'])

            if self.unit.d['inventory_lumber'] + \
                    self.unit.d['inventory_gold'] + \
                    self.unit.d['inventory_oil'] >= self.unit.inventory_space:
                # Inventory is full, return to base
                recall_building = self.unit.closest_recall_building()
                if not recall_building:
                    # Transition to next state since nothing can be done
                    self.transition()
                    return
                else:
                    self.data['harvest_iterator'] = Harvesting.RETURN
                    return

            self.unit.d['inventory_gold'] += tile_data['gold_yield']
            self.unit.d['inventory_lumber'] += tile_data['lumber_yield']
            self.unit.d['inventory_oil'] += tile_data['oil_yield']

            # Todo, adding statistics here becaue its thought to be better for RL????
            self.player.statistics['gold_count'] += tile_data['gold_yield']
            self.player.statistics['lumber_count'] += tile_data['lumber_yield']
            self.player.statistics['oil_count'] += tile_data['oil_yield']

            self.data['harvest_timer'] = 0

    def stage_recall(self, recall_building):
        # Must walk
        tiles = self.game.AdjacentMap.adjacent_walkable(self.game, recall_building.d['x'] + recall_building.dimension,
                                                       recall_building.d['y'] + recall_building.dimension,
                                                       recall_building.dimension + 1)

        if tiles:
            # Wait for free spot to deliver
            tile = self.unit.shortest_distance(tiles)
            if not self.has_next_state(const.State.ID_Harvesting):
                self.unit.move(*tile)
                self.add_next(const.State.ID_Harvesting, {
                    'harvest_iterator': Harvesting.RETURN,
                }, 1)


    def stage_deliver(self):
        self.unit.player.gold += self.unit.d['inventory_gold']
        self.unit.player.lumber += self.unit.d['inventory_lumber']
        self.unit.player.oil += self.unit.d['inventory_oil']
        self.unit.d['inventory_gold'] = 0
        self.unit.d['inventory_lumber'] = 0
        self.unit.d['inventory_oil'] = 0
        self.data['harvest_iterator'] = Harvesting.HARVEST

    def update(self, tick):
        self.data['harvest_timer'] += tick

        if self.data['harvest_iterator'] == Harvesting.HARVEST:

            distance = self.unit.distance(*self.data['harvest_target'])
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

        elif self.data['harvest_iterator'] == Harvesting.RETURN:
            """
            # Find closest recall building (Typically a town-center or lumber mill
            """
            recall_building = self.unit.closest_recall_building()

            if recall_building is None:
                self.transition()
                return

            distance = self.unit.distance(
                recall_building.d['x'],
                recall_building.d['y'],
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

                if Config.MECHANICS_HARVEST_FOREVER:
                    self.unit.harvest(*self.data['harvest_target'])
                else:
                    self.transition()
