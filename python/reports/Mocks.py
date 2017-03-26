
class MockMap:

    def __init__(self):
        self.height = 0
        self.width = 0

    def get_spawn_tile(self, id):
        return (0,0)

class MockUnitManager:
    BASE_1 = None
    FARM = None
    BARRACKS = None
    UNITS = {None:{
        0: None,
        1: None
    }}

class MockGame:

    def __init__(self):
        self.Map = MockMap()
        self.data = {
            'tile': [],
            'unit': []
        }
        self.UnitManager = MockUnitManager
        self.clock = None