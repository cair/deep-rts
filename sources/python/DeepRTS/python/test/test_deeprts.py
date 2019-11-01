import unittest
from DeepRTS import python
from DeepRTS import Engine


class TestDeepRTSGame(unittest.TestCase):

    def setUp(self) -> None:
        self.game = python.Game(
            python.Config.Map.FIFTEEN,
            n_players=1,
            engine_config=None,
            gui_config=None,
        )

    def test_1(self):
        self.assertTrue(self.game.players)