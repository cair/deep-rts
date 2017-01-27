



class Event:
    callbacks = []

    New_State = 0
    Player_Spawn = 1
    Unit_Died = 2
    Retaliate = 3
    Attack = 4
    Unit_Spawn = 5
    Unit_Start_Harvesting = 6
    Unit_Stop_Harvesting = 7
    Unit_Set_Position = 8
    Unit_Move_Harvestable = 9
    Unit_Move = 10
    Unit_Move_Attack = 11
    Unit_Build = 12

    class Feedback:
        Neutral = 0


    @staticmethod
    def notify(event_type, data=None):

        for callback in Event.callbacks:
            callback(event_type, data)