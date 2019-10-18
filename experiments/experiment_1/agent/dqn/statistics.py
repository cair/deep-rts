import numpy
from matplotlib.backends.backend_agg import FigureCanvasAgg
from matplotlib.figure import Figure

class Statistics:

    def __init__(self, action_size, action_names):
        self.action_size = action_size
        self.action_names = action_names
        self.loss = {"x": [], "y": []}
        self.win_percent = {"x": [], "y": []}
        self.action_distribution = {"x": [], "y": []}

        self.episodic_action_distribution = numpy.zeros(action_size, dtype=numpy.int16)

        self.figure = Figure()
        canvas = FigureCanvasAgg(self.figure)

        self.plot_loss = self.figure.add_subplot(2, 2, 1)
        self.plot_win_percent = self.figure.add_subplot(2, 2, 2)
        self.plot_action_distribution = self.figure.add_subplot(2, 2, 3)

        self.win_percent_label = "Win Percent"

    def add_loss(self, epoch, loss):
        self.loss["x"].append(epoch)
        self.loss["y"].append(loss)

    def add_win_percent(self, win_percent, y_axis_name=None):
        if y_axis_name is not None:
            self.win_percent_label = y_axis_name

        self.win_percent["x"].append(len(self.win_percent["x"]) + 1)
        self.win_percent["y"].append(win_percent)

    def add_action(self, a):
        self.episodic_action_distribution[a] = self.episodic_action_distribution[a] + 1

    def next_episode(self, episode):

        population = numpy.sum(self.episodic_action_distribution)
        items = [(x / population) * 100 for x in self.episodic_action_distribution]

        self.action_distribution["y"].append(items)
        self.action_distribution["x"].append(episode)

        self.episodic_action_distribution = numpy.zeros(self.action_size, dtype=numpy.int16)

    def plot(self):
        # Action Distributio

        self.plot_loss.cla()
        self.plot_win_percent.cla()
        self.plot_action_distribution.cla()

        self.plot_loss.grid(True)
        self.plot_loss.set_xlabel("Epoch")
        self.plot_loss.set_ylabel("Loss")

        self.plot_win_percent.grid(True)
        self.plot_win_percent.set_xlabel("Episode")
        self.plot_win_percent.set_ylabel(self.win_percent_label)

        self.plot_action_distribution.grid(True)
        self.plot_action_distribution.set_xlabel("Episode")
        self.plot_action_distribution.set_ylabel("Action Frequency")

        self.plot_loss.plot(self.loss["x"], self.loss["y"])
        self.plot_win_percent.plot(self.win_percent["x"], self.win_percent["y"])

        lineObjects = self.plot_action_distribution.plot(
            numpy.array(self.action_distribution["x"]),
            numpy.array(self.action_distribution["y"])
        )
        self.plot_action_distribution.legend(
            lineObjects,
            self.action_names,
            ncol=2,
            loc=9,
            bbox_to_anchor=(1.6, 1.03),
            fontsize=6
        )

        self.figure.tight_layout()

        self.figure.savefig("output.png")
        self.figure.savefig("output.eps")