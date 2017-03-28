import time


class GameClock:
    def __init__(self):

        self._render = None
        self._update = None

        self._render_interval = None
        self._update_interval = None

        self._render_next = time.time()
        self._update_next = time.time()

        self.max_ups = 1
        self.max_fps = 1
        self.fps = 1
        self.ups = 1
        self._stats_next = time.time()
        self._stats_interval = 1
        self._stats_render = 0
        self._stats_update = 0

        self._shedules = []

        self._update_ticks = 0
        self._render_ticks = 0

    def reset(self):
        self._render_next = time.time()
        self._update_next = time.time()
        self._stats_next = time.time()
        self._stats_render = 0
        self._stats_update = 0
        self._update_ticks = 0
        self._render_ticks = 0

    def toJSON(self):
        return dict(
            _stats_next=self._stats_next,
            _stats_render=self._stats_render,
            _stats_update=self._stats_update,
            _update_ticks=self._update_ticks,
            _render_ticks=self._render_ticks
        )

    def load(self, data):
        self._render_next = time.time()
        self._update_next = time.time()
        self._stats_render = data['_stats_render']
        self._stats_update = data['_stats_update']
        self._update_ticks = data['_update_ticks']
        self._render_ticks = data['_render_ticks']

    def shedule(self, func, interval):
        self._shedules.append([func, interval, time.time(), len(self._shedules)])

    def render(self, func, interval):
        self._render = func
        self._render_interval = 1.0 / interval if interval > 0 else 0

    def update(self, func, interval):
        self._update = func
        self._update_interval = 0 if interval >= 2000 else 1.0 / interval

    @property
    def elapsed_update(self):
        return self._update_ticks

    def tick_schedule(self):
        """
        Tick on scheduled tasks
        Used in order to get Parallell Processes to still run the on_event
        This is because Threads does not work in Multiprocessing
        :return:
        """
        current_time = time.time()
        for (shedule_func, interval, next, idx) in self._shedules:
            if current_time > next:
                shedule_func(1)
                self._shedules[idx][2] = current_time + interval

    def tick(self):
        current_time = time.time()

        if current_time >= self._update_next:
            self._update(1)
            self._update_next = current_time + self._update_interval
            self._update_ticks += 1

        if current_time >= self._render_next:
            self._render()
            self._render_next = current_time + self._render_interval
            self._render_ticks += 1

        for (shedule_func, interval, next, idx) in self._shedules:
            if current_time > next:
                shedule_func()
                self._shedules[idx][2] = current_time + interval

        if current_time >= self._stats_next:
            du = self._update_ticks - self._stats_update
            self._stats_update = self._update_ticks
            dr = self._render_ticks - self._stats_render
            self._stats_render = self._render_ticks
            self._stats_next = current_time + self._stats_interval
            self.ups = du
            self.fps = dr
