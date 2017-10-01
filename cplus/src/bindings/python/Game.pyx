from cython.operator cimport dereference as deref

from libcpp.memory cimport unique_ptr
from libcpp.string cimport string


cdef extern from "../../game/player/Player.h":
    cdef cppclass Player:
        Player(Game&, int)


cdef extern from "../../game/Game.h":

    cdef cppclass Game:
        int fps
        int ups
        Game(int, bool)
        void loop()
        void initGUI()
        void start()
        void createPlayers()







cdef class DeepRTS:
    cdef unique_ptr[Game] thisptr

    def __init__(self, n_players, setup):
        self.thisptr.reset(new Game(n_players, setup))

    def loop(self):
        deref(self.thisptr).loop()

    def initGUI(self):
        deref(self.thisptr).initGUI()

    def start(self):
        deref(self.thisptr).start()

    def createPlayers(self):
        deref(self.thisptr).createPlayers()

    property fps:
        def __get__(self):
            return deref(self.thisptr).fps
        def __set__(self, _fps):
            deref(self.thisptr).fps = _fps

    property ups:
        def __get__(self):
            return deref(self.thisptr).ups
        def __set__(self, _ups):
            deref(self.thisptr).ups = _ups