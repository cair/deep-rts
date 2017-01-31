from Mechanics.API.Action import Action
from Mechanics.API.Event import Event


class AI:

    base_area = []
    base_wood_plots = []
    base_gold_plots = []

    def __init__(self, player):
        self.player = player

        # Event Handlers
        self.player.Event.on_event(self.on_event)
        self.player.Event.on_start(self.on_start)
        self.player.Event.on_defeat(self.on_defeat)
        self.player.Event.on_victory(self.on_victory)
        self.player.Event.on_init(self.on_init)

        # API Endpoints
        self.Action = Action(self.player)

    def on_init(self, game):
        pass

    def on_start(self):
        pass

    def on_victory(self):
        pass

    def on_defeat(self):
        pass

    def on_event(self, event, data=None):


        if event == Event.New_State:
            pass
        elif event == Event.Player_Spawn:
            pass #print("Player Spawn")
        elif event == Event.Unit_Died:
            pass #print("Unit Died")
        elif event == Event.Retaliate:
            pass #print("Unit Retaliate")
        elif event == Event.Attack:
            pass #print("Unit Attack")
        elif event == Event.Unit_Spawn:
            pass #print("Unit Spawn")
        elif event == Event.Unit_Start_Harvesting:
            pass #print("Unit Start Harvesting")
        elif event == Event.Unit_Stop_Harvesting:
            pass #print("Unit Stop Harvesting")
        elif event == Event.Unit_Set_Position:
            pass # pass #print("Unit Set Position")
        elif event == Event.Unit_Move_Harvestable:
            pass #print("Unit Move Harvestable")
        elif event == Event.Unit_Move:
            pass #print("Unit Move")
        elif event == Event.Unit_Move_Attack:
            pass #print("Unit Move Attack")
        elif event == Event.Unit_Build:
            pass #print("Unit Build")
