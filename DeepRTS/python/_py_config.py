

class Config:

    DEFAULTS = dict(
        render=True,
        view=False,
        inputs=False,
        caption=False,
        unit_health=True,
        unit_outline=True,
        unit_animation=True,
        audio=False,
        audio_volume=50
    )

    class Map:
        TEN = "10x10-2v2.json"
        FIFTEEN = "15x15-2v2.json"
        TWENTYONE = "21x21-2v2.json"
        THIRTYONE = "31x31-2v2.json"
        THIRTYONE_FOUR = "31x31-4v4.json"
        THIRTYONE_SIX = "31x31-6v6.json"

    def __init__(
            self,
            render=DEFAULTS["render"],
            view=DEFAULTS["view"],
            inputs=DEFAULTS["inputs"],
            caption=DEFAULTS["caption"],
            unit_health=DEFAULTS["unit_health"],
            unit_outline=DEFAULTS["unit_outline"],
            unit_animation=DEFAULTS["unit_animation"],
            audio=DEFAULTS["audio"],
            audio_volume=DEFAULTS["audio_volume"]
    ):
        self.input = inputs
        self.render = render
        self.view = view
        self.caption = caption
        self.unit_health = unit_health
        self.unit_animation = unit_animation
        self.unit_outline = unit_outline
        self.audio = audio
        self.audio_volume = audio_volume
