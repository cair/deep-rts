
class GreedyMCTS:


    def __init__(self,
                 available_time,
                 max_playouts,
                 lookahead,
                 max_depth,
                 e1,
                 discount1,
                 e2,
                 discount2,
                 e3,
                 discount3,
                 policy,
                 eval_f):

        self.available_time = available_time
        self.max_playouts = max_playouts
        self.max_simulation_time = lookahead
        self.max_depth = max_depth

        self.initial_epsilon_l = e1
        self.initial_epsilon_g = e2
        self.initial_epsilon_0 = e3
        self.discount_l = discount1
        self.discount_g = discount2
        self.discount_0 = discount3
        self.policy = policy
        self.ef = eval_f


    def reset(self):
        pass

        """tree = None;
        gs_to_start_from = null;
        total_runs = 0;
        total_cycles_executed = 0;
        total_actions_issued = 0;
        total_time = 0;
        current_iteration = 0;
        }"""