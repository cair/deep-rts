import random
import uuid


class OvertimeStrategy:
    SUDDEN_DEATH = 1


class Strategy:
    SINGLE_ELIMINATION = 1
    DOUBLE_ELIMINATION = 2


class SortStrategy:
    RANK = lambda p: p.rank
    RANDOM = lambda p: random.random()


class Tournament:
    class PlayerInfo:

        def __init__(self, player, rank):
            self._id = str(uuid.uuid4())
            self.agent = player
            self._rank = rank

        @property
        def rank(self):
            return self._rank

        @property
        def pid(self):
            return self._id

        @property
        def is_winner(self):
            return self.agent.env.winner == self.agent.player

        def __repr__(self):
            return self.pid

    class Match:
        def __init__(self,
                     next_match,
                     env,
                     n_participants,
                     games,
                     tournament_strategy,
                     overtime_strategy=OvertimeStrategy.SUDDEN_DEATH):
            self._next_match = next_match

            self._match_id = None
            self._match_round = None

            self._n_participants = n_participants
            self._participants = []
            self._tournament_strategy = tournament_strategy
            self._overtime_strategy = overtime_strategy
            self._games = games

            self._env = env
            self._winner = None
            self._logbook = {}

        def set_match_id(self, match_id):
            self._match_id = match_id

        def set_match_round(self, match_round):
            self._match_round = match_round

        def add_participant(self, participant):
            self._participants.append(participant)

        @property
        def n_participants(self):
            return self._n_participants

        @property
        def match_round(self):
            return self._match_round

        @property
        def participant_ids(self):
            return [p.pid for p in self._participants]

        @property
        def match_id(self):
            return self._match_id

        @property
        def next_match(self):
            return self._next_match

        def get_logbook(self):
            return self._logbook

        def prepare_match(self):
            assert self._winner is None
            """
            This function sets env and player object for each participant.
            :return:
            """
            self._logbook = {"players": {
                participant.pid: dict(
                    match_results=[],
                ) for participant in self._participants
            }, "winner": None}
            for i, participant in enumerate(self._participants):
                participant.agent.set_env(self._env)
                participant.agent.set_player(self._env.players[i])

        def play(self):
            print(f"[ROUND={self._match_round}|MATCH={self._match_id},GAMES={self._games}]Playing  - {' vs '.join(self.participant_ids)}")
            for episode in range(self._games):
                print(f"[GAME#{episode}]: ")
                self.play_episode(self._env)

            winner = None
            extra_count = 0
            while winner is None or (extra_count > 0 and self._overtime_strategy == OvertimeStrategy.SUDDEN_DEATH):
                winner = self.evaluate_winner()

                if winner is None:
                    self.play_episode(self._env)
                    extra_count += 1

            assert (winner is not None)
            self._winner = winner

            self._logbook["winner"] = winner

            return self.get_logbook()

        def evaluate_winner(self):
            """
                Find the guy with most wins. if two of the top players has equal wins, play more (with all players)
                :return:
                """
            most_wins_participant = None
            for participant in self._participants:
                participant_wins = self._logbook["players"][participant.pid]["match_results"].count(1)
                if most_wins_participant is None:
                    most_wins_participant = [participant, participant_wins]
                    continue

                if participant_wins > most_wins_participant[1]:
                    most_wins_participant = [participant, participant_wins]
                elif participant_wins == most_wins_participant[1]:
                    # draw between "at least 2 players. lets just play more # todo might be totally wrong
                    return None

            # Return winner.
            return most_wins_participant[0]

        def play_episode(self, env):
            env.reset()

            while not env.is_terminal():

                for participant in self._participants:
                    agent = participant.agent
                    # Get initial observation
                    obs = agent.get_state()

                    # Preprocess state
                    obs = agent.state_preprocess(obs)

                    # Predict action (optional)
                    action = agent.get_action(obs)

                    # Make action
                    obs1, r, t, info = env.step(action, player=agent.player)

                    # Preprocess resulting state (optional)
                    obs1 = agent.state_preprocess(obs1)

                    # Memorize (optional)
                    agent.memorize(obs, action, obs1, r, t, info)

            for participant in self._participants:
                self._logbook["players"][participant.pid]["match_results"].append(1 if participant.is_winner else 0)

        @property
        def winner(self):
            return self._winner

        def __str__(self):
            return f"Match: id={self._match_id}"

    def __init__(self,
                 env_cls,
                 players_per_match=2,
                 byes_enabled=True,
                 best_of_n_matches=3,
                 default_player_rank=0.5,
                 tournament_strategy=Strategy.SINGLE_ELIMINATION,
                 initial_matchup_strategy=SortStrategy.RANK
                 ):

        self._env_cls = env_cls
        self._players_per_match = players_per_match
        self._byes_enabled = byes_enabled
        self._best_of_n_matches = best_of_n_matches
        self._default_player_rank = default_player_rank
        self._tournament_is_started = False
        self._tournament_strategy = tournament_strategy
        self._tournament_initial_matchup_strategy = initial_matchup_strategy

        self._roster = []  # The full player roster
        self._matches = None  # A list of lists with matches. each inner list represent a round
        self._round_iterator = 0  # The iterator that signifies which round we are currently on.

    def create_tournament_tree(self, next_match=None):
        env = self._env_cls()  # TODO we are creating same env for all players. This is prolly fine.
        result = []
        leaf_nodes = [Tournament.Match(
            next_match,
            env,
            self._players_per_match,
            self._best_of_n_matches,
            self._tournament_strategy
        )]
        result.insert(0, leaf_nodes)

        leaf_budget = len(self._roster)  # Maximum number of leaf nodes. This is the same as roster size
        current_leaf_count = 0
        # Backward pass (generate tree)
        while True:

            potential_new_count = (leaf_budget / self._players_per_match) - max(0, (current_leaf_count - self._players_per_match))
            if potential_new_count <= 0:
                break

            new_leaf_nodes = []

            if len(leaf_nodes) > potential_new_count:
                # Move to leaf position those we do not expand further
                new_leaf_nodes = leaf_nodes[:int(potential_new_count)]
                leaf_nodes = leaf_nodes[int(potential_new_count):]
                result[0] = leaf_nodes

            for l_i, leaf in enumerate(leaf_nodes):

                for i in range(self._players_per_match):
                    new_leaf_nodes.insert(0, Tournament.Match(
                        leaf,
                        env,
                        self._players_per_match,
                        self._best_of_n_matches,
                        self._tournament_strategy
                    ))
            current_leaf_count += len(new_leaf_nodes) * self._players_per_match
            result.insert(0, new_leaf_nodes)
            leaf_nodes = new_leaf_nodes

        # forward pass (generate id and depth)
        match_id = 1
        queue = leaf_nodes[:]
        for match in queue:
            match.set_match_round(match_round=1)

        while len(queue):
            item = queue.pop(0)

            if item.match_id is None:
                item.set_match_id(match_id)
                match_id += 1

            next_match = item.next_match
            if next_match:
                next_match.set_match_round(match_round=item.match_round + 1)
                queue.append(next_match)

        return result

    def start(self):
        if len(self._roster) < 2:
            raise RuntimeError("A tournament should have AT LEAST 2 players")
        elif not self._byes_enabled and not Tournament.is_pow_2(len(self._roster)):
            raise RuntimeError("Byes are not enabled, and the player pool is not a power of 2")
        elif self._tournament_is_started:
            raise RuntimeError("Cannot start tournament twice!")

        self._matches = self.create_tournament_tree(next_match=None)
        self._populate_initial_round()

        while self._round_iterator < len(self._matches):
            self.play_round()
            self._round_iterator += 1

    def generate_matchup_from_results(self, results):
        pass

    def play_round(self):
        assert self._matches is not None

        for match in self._matches[self._round_iterator]:
            match.prepare_match()
            result = match.play()

            if match.next_match:
                match.next_match.add_participant(result["winner"])

    def _populate_initial_round(self):

        def divide_chunks(l, n):
            # looping till length l
            for i in range(0, len(l), n):
                yield l[i:i + n]

        player_split = list(divide_chunks(sorted(self._roster, key=self._tournament_initial_matchup_strategy),
                                          self._players_per_match))

        for match_i, match in enumerate(self._matches[0]):
            participants = player_split[match_i]
            assert len(participants) == match.n_participants
            for participant in participants:
                match.add_participant(participant)

    def add_agent(self, agent_cls, rank=None):
        if self._tournament_is_started:
            raise RuntimeError("Cannot add players after the tournament is started.")
        rank = rank if rank else self._default_player_rank

        self._roster.append(Tournament.PlayerInfo(
            player=agent_cls(env=None),
            rank=rank
        ))

    @staticmethod
    def is_pow_2(n):
        return (n & (n - 1)) == 0
