#!/usr/bin/env python

# This file is part of GameClock.
#
# GameClock is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# GameClock is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with GameClock.  If not, see /www.gnu.org/licenses/>.

# Compatible: Python 2.7, Python 3.2

"""gameclock.py - Game clock for Gummworld2.

GameClock is a fixed time-step clock that keeps time in terms of game
time. It will attempt to keep game time close to real time, so if an
interval takes one second of game time then the user experiences one
second of real time. In the worst case where the CPU cannot keep up with
the game load, game time will subjectively take longer but still remain
accurate enough to keep game elements in synchronization.

GameClock manages time in the following ways:

    1.  Register special callback functions that will be run when they
        are due.
    2.  Schedule game logic updates at a constant speed, independent of
        frame rate.
    3.  Schedule frames at capped frames-per-second, or uncapped.
    4.  Invoke a pre-emptive pause callback when paused.
    5.  Schedule miscellaneous items at user-configurable intervals.
    6.  Optionally sleep between schedules to conserve CPU cycles.
    7.  Gracefully handle corner cases.

Note the Python Library docs mention that not all computer platforms'
time functions return time in fractions of a second. This module will not
work on such platforms.

USAGE

Callback:

    clock = GameClock(
        update_callback=update_world,
        frame_callback=draw_scene,
        pause_callback=pause_game)
    while 1:
        clock.tick()

Special callbacks can be directly set and cleared at any time:

    clock.update_callback = my_new_update
    clock.frame_callback = my_new_draw
    clock.pause_callback = my_new_pause

    clock.update_callback = None
    clock.frame_callback = None
    clock.pause_callback = None

Scheduling miscellanous callbacks:

    def every_second_of_every_day(dt):
        "..."
    clock.schedule_interval(every_second_of_every_day, 1.0)

Callbacks can be any kind of callable that accepts the callback signature.

The update_callback receives a single DT argument, which is the time-step
in seconds since the last update.

The frame_callback receives a single INTERPOLATION argument, which is the
fractional position in time of the frame within the current update time-
step. It is a float in the range 0.0 to 1.0.

The pause_callback receives no arguments.

User-defined interval callbacks accept at least a DT argument, which is
the scheduled item's interval, and optional user-defined arguments. See
GameClock.schedule_interval.

DEPRECATIONS

Old Style Game Loop

Use of the old style game loop is deprecated. Don't do this anymore:

    if clock.update_ready:
        update(clock.dt_update)
    if clock.frame_ready:
        draw(clock.interpolate)

The old style game loop will work on sufficiently fast hardware. Timekeeping
will break on slow hardware that cannot keep up with a heavy workload. This is
because the old style ignores the cost of the frame routine. By using callbacks
instead, cost is factored into the frame scheduling and overloading the CPU has
fewer negative side effects.

The update_ready and frame_ready public attributes have been removed.

CREDITS

The inspiration for this module came from Koen Witters's superb article
"deWiTTERS Game Loop", aka "Constant Game Speed independent of Variable FPS"
at http://www.koonsolo.com/news/dewitters-gameloop/.

The clock was changed to use a fixed time-step after many discussions with
DR0ID, and a few readings of
http://gafferongames.com/game-physics/fix-your-timestep/.

Thanks to Koen Witters, DR0ID, and Glenn Fiedler for sharing.

Pythonated by Gummbum. While the builtin demo requires pygame, the module
does not. The GameClock class is purely Python and should be compatible with
other Python-based multi-media and game development libraries.
"""

__version__ = '$Id: gameclock.py 428 2013-08-28 05:43:47Z stabbingfinger@gmail.com $'
__author__ = 'Gummbum, (c) 2011-2014'


import functools
import time


class _IntervalItem(object):
    """An interval item runs after an elapsed interval."""
    # __slots__ = ['func', 'interval', 'lasttime', 'life', 'args', 'id']
    id = 0
    def __init__(self, func, interval, curtime, life, args):
        self.func = func
        self.interval = float(interval)
        self.lasttime = curtime
        self.life = life
        self.args = args
        self.id = _IntervalItem.id
        _IntervalItem.id += 1


class GameClock(object):

    def __init__(self,
                 max_ups=100,
                 max_fps=0,
                 use_wait=False,
                 time_source=time.time,
                 update_callback=None,
                 frame_callback=None,
                 paused_callback=None):

        # Configurables.
        self.get_ticks = time_source
        self.max_ups = max_ups
        self.max_fps = max_fps
        self.use_wait = use_wait
        self.update_callback = update_callback
        self.frame_callback = frame_callback
        self.paused_callback = paused_callback

        self._start_time = time.time()

        # Time keeping.
        TIME = self.get_ticks()
        self._real_time = TIME
        self._game_time = TIME
        self._last_update = TIME
        self._last_update_real = TIME
        self._next_update = TIME
        self._last_frame = TIME
        self._next_frame = TIME
        self._next_second = TIME
        self._update_ready = False
        self._frame_ready = False
        self._paused = 0

        # Schedules
        self._need_sort = False
        self._schedules = []
        self._unschedules = []

        # Metrics: update and frame progress counter in the current one-second
        # interval.
        self.num_updates = 0
        self.num_frames = 0
        # Metrics: duration in seconds of the previous update and frame.
        self.dt_update = 0.0
        self.dt_frame = 0.0
        # Metrics: how much real time a callback consumes
        self.cost_of_update = 0.0
        self.cost_of_frame = 0.0
        # Metrics: average updates and frames per second over the last five
        # seconds.
        self.ups = 0.0
        self.fps = 0.0

    @property
    def max_ups(self):
        return self._max_ups
    @max_ups.setter
    def max_ups(self, val):
        self._max_ups = val
        self._update_interval = 1.0 / val

    @property
    def max_fps(self):
        return self._max_fps
    @max_fps.setter
    def max_fps(self, val):
        self._max_fps = val
        self._frame_interval = 1.0 / val if val>0 else 0

    @property
    def game_time(self):
        """Virtual elapsed time in game milliseconds.
        """
        return self._game_time

    @property
    def elapsed(self):
        return self._game_time - self._start_time

    @property
    def paused(self):
        """The real time at which the clock was paused, or zero if the clock
        is not paused.
        """
        return self._paused

    @property
    def interpolate(self):
        interp = (self._real_time - self._last_update_real) / self._update_interval
        if interp < 0.0:
            return 0.0
        if interp > 1.0:
            return 1.0
        return interp

    def tick(self):
        # Now.
        real_time = self.get_ticks()
        self._real_time = real_time

        # Pre-emptive pause callback.
        if self._paused:
            if self.paused_callback:
                self.paused_callback()
            return

        # Check if update and frame are due.
        update_interval = self._update_interval
        game_time = self._game_time
        if real_time >= self._next_update:
            self.dt_update = update_interval  # fixed timestep: good
            self._last_update_real = real_time
            game_time += update_interval
            self._game_time = game_time
            self._last_update = game_time
            self._next_update = real_time + update_interval
            self.num_updates += 1
            if self.update_callback:
                self._update_ready = True
        if real_time - self._last_frame >= self._update_interval or (real_time + self.cost_of_frame < self._next_update and
        real_time >= self._next_frame):
            self.dt_frame = real_time - self._last_frame
            self._last_frame = real_time
            self._next_frame = real_time + self._frame_interval
            self.num_frames += 1
            if self.frame_callback:
                self._frame_ready = True

        # Check if a schedule is due, and when.
        sched_ready = False
        sched_due = 0
        if self._schedules:
            sched = self._schedules[0]
            sched_due = sched.lasttime + sched.interval
            if real_time >= sched_due:
                sched_ready = True

        # Run schedules if any are due.
        if self._update_ready or sched_ready:
            self._run_schedules()

        # Run the frame callback (moved inline to reduce function calls).
        if self.frame_callback and self._frame_ready:
            get_ticks = self.get_ticks
            t = get_ticks()
            self.frame_callback(self.interpolate)
            self.cost_of_frame = get_ticks() - t
            self._frame_ready = False

        # Flip metrics counters every second.
        if real_time >= self._next_second:
            self._flip(real_time)

        # Sleep to save CPU.
        if self.use_wait:
            upcoming_events = [
                self._next_frame,
                self._next_update,
                self._next_second,
            ]
            if sched_due != 0:
                upcoming_events.append(sched_due)
            next_due = functools.reduce(min, upcoming_events)
            t = self.get_ticks()
            time_to_sleep = next_due - t
            if time_to_sleep >= 0.002:
                time.sleep(time_to_sleep)

    def pause(self):
        """Pause the clock so that time does not elapse.

        While the clock is paused, no schedules will fire and tick() returns
        immediately without progressing internal counters. Game loops that
        completely rely on the clock will need to take over timekeeping and
        handling events; otherwise, the game will appear to deadlock. There are
        many ways to solve this scenario. For instance, another clock can be
        created and used temporarily, and the original swapped back in and
        resumed when needed.
        """
        self._paused = self.get_ticks()

    def resume(self):
        """Resume the clock from the point that it was paused."""
        real_time = self.get_ticks()
        paused = self._paused
        for item in self._schedules:
            dt = paused - item.lasttime
            item.lasttime = real_time - dt
        self._last_update_real = real_time - (paused - self._last_update_real)
        self._paused = 0
        self._real_time = real_time

    def schedule_interval(self, func, interval, life=0, args=[]):
        """Schedule an item to be called back each time an interval elapses.

        While the clock is paused time does not pass.

        Parameters:
            func -> The callback function.
            interval -> The time in seconds (float) between calls.
            life -> The number of times the callback will fire, after which the
                schedule will be removed. If the value 0 is specified, the event
                will persist until manually unscheduled.
            args -> A list that will be passed to the callback as an unpacked
                sequence, like so: item.func(*[item.interval]+item.args).

        """
        # self.unschedule(func)
        item = _IntervalItem(
            func, interval, self.get_ticks(), life, [interval]+list(args))
        self._schedules.append(item)
        self._need_sort = True
        return item.id

    def unschedule(self, func):
        """Unschedule managed functions. All matching items are removed."""
        sched = self._schedules
        for item in list(sched):
            if item.func == func:
                sched.remove(item)

    def unschedule_by_id(self, id):
        """Unschedule a single managed function by the unique ID that is
        returned by schedule_interval().
        """
        sched = self._schedules
        for item in list(sched):
            if item.id == id:
                sched.remove(item)
    @staticmethod
    def _interval_item_sort_key(item):
        return item.lasttime + item.interval

    def _run_schedules(self):
        get_ticks = self.get_ticks

        # Run the update callback.
        if self.update_callback and self._update_ready:
            t = get_ticks()

            self.update_callback[0](self.dt_update)
            self.cost_of_update = get_ticks() - t
            self._update_ready = False

        # Run the interval callbacks.
        if self._need_sort:
            self._schedules.sort(key=self._interval_item_sort_key)
            self._need_sort = False
        real_time = self._real_time
        for sched in self._schedules:
            interval = sched.interval
            due = sched.lasttime + interval
            if real_time >= due:
                sched.func(*sched.args)
                sched.lasttime += interval
                need_sort = True
                if sched.life > 0:
                    if sched.life == 1:
                        self._unschedules.append(sched.id)
                        need_sort = False
                    else:
                        sched.life -= 1
                if need_sort:
                    self._need_sort = True
            else:
                break
        if self._unschedules:
            for id in self._unschedules:
                self.unschedule_by_id(id)
            del self._unschedules[:]

    def _flip(self, real_time):
        self.ups = self.num_updates
        self.fps = self.num_frames

        self.num_updates = 0
        self.num_frames = 0

        self._last_second = real_time
        self._next_second += 1.0