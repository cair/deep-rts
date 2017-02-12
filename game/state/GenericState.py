from game.util.PriorityQueue import PriorityQueue
from game.const import Unit

class GenericState:
    type = None
    default = None

    registry = {}

    def init(self):
        pass

    def clear_next(self):
        self.next_states = PriorityQueue([])

    def has_next_state(self, state):
        for s in self.next_states:
            if state == GenericState.registry[s[0]]:
                return True
        return False

    def __repr__(self):
        return '<%s@%s>' % (
            self.__class__.__name__,
            hex(id(self))
        )

    def toJSON(self):
        data = {}
        for (k, v) in self.attributes.items():
            data[k] = getattr(self, k, None)
        data.update(self.toJSON2())
        data['type'] = self.type
        data['next_states'] = self.next_states.toJSON()
        return data

    def toJSON2(self):
        return {}

    def __init__(self, unit, attributes):
        self.unit = unit
        self.player = self.unit.player
        self.Event = self.unit.Event
        self.game = unit.game
        self.Map = self.game.Map
        self.next_states = PriorityQueue([])

        self.attributes = {
            'x': None,
            'y': None,
            'tick': 0,
            'inventory_lumber': 0,
            'inventory_gold': 0,
            'inventory_oil': 0,
            'spawned': False,
            'health': None,
            'direction': Unit.DOWN
        }

        for (k, v) in self.attributes.items():
            setattr(self, k, v)
        for (k, v) in attributes.items():
            setattr(self, k, v)

        # Transform next_states to priorityqueue
        if 'next_states' in attributes:
            self.next_states = PriorityQueue(attributes['next_states'])

    def transition(self, force_state=None, attributes={}):

        if force_state:
            next_state = force_state(self.unit, attributes=attributes)
            next_state.init()
        else:
            d = self.next_states.get()
            next_state = None
            if d:
                state_id, attributes = d
                next_state = GenericState.registry[state_id](self.unit, attributes=attributes)
                next_state.init()

        if next_state:
            next_state.next_states = self.next_states
            for (k, v) in self.attributes.items():
                setattr(next_state, k, getattr(self, k, None))

            #print("%s. Transition from %s | %s => %s (%s => %s)" % (self.tick, self.unit, self.id, next_state.id, self, next_state))
            self.unit.state = next_state
        else:
            if self.default:
                self.transition(self.default)

    def update(self, dt):
        pass

    def add_next(self, state_clazz, attributes, pri):
        #state = state_clazz(self.unit, attributes=attributes)

        self.next_states.put((state_clazz.type, attributes), pri)
