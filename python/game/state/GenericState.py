from game.util.PriorityQueue import PriorityQueue
from game import const

class GenericState:
    type = None
    default = None

    registry = {}

    def init(self):
        pass


    def has_next_state(self, state_id):

        for sid in self.unit.states['q']:
            if sid == state_id:
                return True
        return False

    def __repr__(self):
        return '<%s@%s>' % (
            self.__class__.__name__,
            hex(id(self))
        )

    def __init__(self, unit):
        self.unit = unit
        self.player = self.unit.player
        self.Event = self.unit.Event
        self.game = unit.game
        self.Map = self.game.Map
        self.next_states = self.unit.states['q']

    def transition(self, state_id=None, attributes={}):
        """
        # Transitions to a state, if state parameter is set, it is forced
        :param state_id:
        :param attributes:
        :return:
        """

        if state_id is not None:
            self.unit.state = self.unit.states[state_id]
            self.unit.d['s'][state_id].update(attributes)
            self.unit.d['current_state'] = state_id
            self.unit.state.init()
            #print("(%s) %s ==> %s" % (self.unit, self.id, self.unit.state.id))
            return True

        state_data = self.next_states.get()

        if state_data:
            state_id, attributes = state_data
            self.unit.state = self.unit.states[state_id]
            self.unit.d['s'][state_id].update(attributes)
            self.unit.d['current_state'] = state_id
            self.unit.state.init()
            #print("(%s) %s ==> %s" % (self.unit, self.id, self.unit.state.id))
            return True

        self.unit.state = self.unit.states[const.State.ID_Idle]
        self.unit.d['current_state'] = const.State.ID_Idle
        #print("(%s) %s ==> %s" % (self.unit, self.id, self.unit.state.id))

        return True

    def update(self, dt):
        pass

    def add_next(self, state_id, attributes, pri):
        self.next_states.put((state_id, attributes), pri)
