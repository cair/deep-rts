import random
import uuid
import copy


class OvertimeStrategy:
    SUDDEN_DEATH = 1


class Strategy:
    SINGLE_ELIMINATION = 1
    # DOUBLE_ELIMINATION = 2
    SWISS = 3


class SortStrategy:
    RANK = lambda p: p.rating
    RANDOM = lambda p: random.random()


class Elo:
    # based off  - https://github.com/zlypher/elo-rating/blob/master/src/elo-rating.js
    @staticmethod
    def rating_difference(player_rating, opponent_rating):
        """
             * Calculates the difference between two ratings.
             * The difference is capped at +400 and -400.
        :param player_rating:
        :param opponent_rating:
        :return:
        """
        return max(min(player_rating - opponent_rating, 400), -400)

    @staticmethod
    def expected(player_rating, opponent_rating):
        """
        * Calculates the expected outcome of a game between two players of the
        * ratings playerRating and opponentRating.
        *
        * Formular: E = 1 / 1 + 10^((OR - PR)/400)
        * E: Expected value
        * PR: Player Rating
        * OR: Opponent Rating
        *
        :param player_rating:
        :param opponent_rating:
        :return:
        """
        return 1 / (1 + 10 ** (Elo.rating_difference(opponent_rating, player_rating)))

    @staticmethod
    def calculate(player_rating, opponent_rating, player_won, k=20):
        """
        * Calculates the new ratings based on the given ratings and a flag
        * indicating if player one has won.
        :param player_rating:
        :param opponent_rating:
        :param player_won:
        :param k:
        :return:
        """
        player_expected = Elo.expected(player_rating, opponent_rating)
        player_won_int = 1 if player_won else 0
        rating_change = k * (player_won_int - player_expected)
        return dict(
            player_rating=player_rating + rating_change,
            opponent_rating=player_rating + rating_change * -1
        )


class Tournament:
    class PlayerInfo:

        def __init__(self, player, rating):
            self._id = str(uuid.uuid4())
            self.agent = player
            self._rating = rating

            self._wins = 0
            self._defeats = 0

        def reset(self):
            self._wins = 0
            self._defeats = 0

        def update_rating(self, new_rating, strategy="set"):

            if strategy == "avg":
                self._rating = (new_rating + self._rating) / 2
            elif strategy == "set":
                self._rating = new_rating
            else:
                raise RuntimeError("strategy not implemented.")

        def increment_defeat(self):
            self._defeats += 1

        def increment_wins(self):
            self._wins += 1

        @property
        def group(self):
            return f"{self._wins}-{self._defeats}"

        @property
        def wins(self):
            return self._wins

        @property
        def defeats(self):
            return self._defeats

        @property
        def rating(self):
            return self._rating

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
        def participants(self):
            return self._participants

        @property
        def match_id(self):
            return self._match_id

        @property
        def next_match(self):
            return self._next_match

        def get_logbook(self):
            return self._logbook

        def prepare_match(self):
            if self._winner is not None:
                raise RuntimeError("Winner must be None else it means that the match is 'dirty'")

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
            if len(self.participants) == 1:
                winner = self.participants[0]
                self._winner = winner
                self._logbook["winner"] = winner
                return self.get_logbook()

            print(
                f"[ROUND={self._match_round}|MATCH={self._match_id},GAMES={self._games}]Playing  - {' vs '.join(self.participant_ids)}")
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

            # Increment loss/wins for each participant
            rating_non_winners = [x.rating for x in self._participants]
            average_defeated_rating = sum(rating_non_winners) / len(rating_non_winners)
            for participant in self._participants:
                if participant == winner:
                    participant.update_rating(Elo.calculate(
                        winner.rating,
                        average_defeated_rating,
                        player_won=True
                    )["player_rating"])
                    participant.increment_wins()
                else:
                    participant.update_rating(Elo.calculate(
                        participant.rating,
                        winner.rating,
                        player_won=False
                    )["player_rating"])
                    participant.increment_defeat()

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

    class SingleEliminationModel:

        def __init__(self, tournament: "Tournament"):
            self._matches = None  # A list of lists with matches. each inner list represent a round
            self._round_iterator = 0  # The iterator that signifies which round we are currently on.
            self.t: "Tournament" = tournament

        def reset(self):
            self._matches = None
            self._round_iterator = 0

        def create_tournament_tree(self, next_match=None):
            env = self.t.env_cls()  # TODO we are creating same env for all players. This is prolly fine.
            result = []
            leaf_nodes = [Tournament.Match(
                next_match,
                env,
                self.t.players_per_match,
                self.t.best_of_n_matches,
                self.t.tournament_strategy
            )]
            result.insert(0, leaf_nodes)

            leaf_budget = len(self.t.roster)  # Maximum number of leaf nodes. This is the same as roster size
            current_leaf_count = 0
            # Backward pass (generate tree)
            while True:

                potential_new_count = (leaf_budget / self.t.players_per_match) - max(0, (
                        current_leaf_count - self.t.players_per_match))
                if potential_new_count <= 0:
                    break

                new_leaf_nodes = []

                if len(leaf_nodes) > potential_new_count:
                    # Move to leaf position those we do not expand further
                    new_leaf_nodes = leaf_nodes[:int(potential_new_count)]
                    leaf_nodes = leaf_nodes[int(potential_new_count):]
                    result[0] = leaf_nodes

                for l_i, leaf in enumerate(leaf_nodes):

                    for i in range(self.t.players_per_match):
                        new_leaf_nodes.insert(0, Tournament.Match(
                            leaf,
                            env,
                            self.t.players_per_match,
                            self.t.best_of_n_matches,
                            self.t.tournament_strategy
                        ))
                current_leaf_count += len(new_leaf_nodes) * self.t.players_per_match
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

        def generate_summary(self):

            summary = dict(
                nodes=[],
                edges=[]
            )

            for match_group in self._matches:
                for match in match_group:

                    summary["nodes"].append(dict(
                        name=match.match_id,
                        data=[dict(
                            round=match.match_round,
                            id=match.match_id,
                            participants=match.participant_ids,
                            winner=match.winner.pid
                        )]
                    ))

                    if match.next_match:
                        summary["edges"].append((match.match_id, match.next_match.match_id))

            return summary

        def start(self):
            if len(self.t.roster) < 2:
                raise RuntimeError("A tournament should have AT LEAST 2 players")
            elif not self.t.byes_enabled and not Tournament.is_pow_2(len(self.t.roster)):
                raise RuntimeError("Byes are not enabled, and the player pool is not a power of 2")
            elif self.t.tournament_is_started:
                raise RuntimeError("Cannot start tournament twice!")

            self._matches = self.create_tournament_tree(next_match=None)
            self._populate_initial_round()

            while self._round_iterator < len(self._matches):
                self.play_round()
                self._round_iterator += 1

            return self.generate_summary()

        def play_round(self):
            assert self._matches is not None

            for match in self._matches[self._round_iterator]:
                match.prepare_match()
                result = match.play()

                if match.next_match:
                    match.next_match.add_participant(result["winner"])

        def _populate_initial_round(self):

            player_split = list(
                self.t.divide_chunks(sorted(self.t.roster, key=self.t.tournament_initial_matchup_strategy),
                                     self.t.players_per_match))

            for match_i, match in enumerate(self._matches[0]):
                participants = player_split[match_i]
                assert len(participants) == match.n_participants
                for participant in participants:
                    match.add_participant(participant)

    class SwissModel:

        def __init__(self, tournament: "Tournament"):

            self.GROUP_TEMPLATE = {
                "finished": False,
                "matches": []
            }

            self.t = tournament
            self.groups = None
            self.env = None
            self.match_counter = None
            self.reset()

        def reset(self):
            if self.env:
                self.env.reset()
            self.match_counter = 1
            self.groups = {
                "0-0": copy.deepcopy(self.GROUP_TEMPLATE)
            }
            self.env = self.t.env_cls()
            self.match_counter = 1

        def start(self):
            # Ensure that all players are 0-0 at the start
            assert [m.group for m in self.t.roster].count("0-0") == len(self.t.roster)

            tournament_done = False
            while not tournament_done:
                for participant in self.t.roster:
                    # generate groups
                    if participant.group not in self.groups:
                        self.groups[participant.group] = copy.deepcopy(self.GROUP_TEMPLATE)

                tournament_done = True
                for group, group_data in filter(lambda x: not x[1]["finished"], self.groups.items()):

                    tournament_done = False

                    group_participants = filter(lambda p: p.group == group, self.t.roster)
                    group_participants = self.t.divide_chunks(
                        sorted(group_participants, key=self.t.tournament_initial_matchup_strategy),
                        self.t.players_per_match)

                    for matched_players in group_participants:

                        self.groups[group]["matches"].append(
                            Tournament.Match(
                                None,
                                self.env,
                                self.t.players_per_match,
                                self.t.best_of_n_matches,
                                self.t.tournament_strategy
                            )
                        )
                        self.groups[group]["matches"][-1].set_match_id(self.match_counter)
                        for p in matched_players:
                            self.groups[group]["matches"][-1].add_participant(p)
                        self.match_counter += 1

                    for match in group_data["matches"]:
                        match.prepare_match()
                        result = match.play()

                    # Group is all played. aka finished.
                    group_data["finished"] = True

            # for player in sorted(self.t.roster, key=lambda p: p.rating, reverse=False):
            #    print(player.group, " - ", player.rating)
            return self.generate_summary()

        def generate_summary(self):

            summary = dict(
                nodes=[],
                edges=[]
            )

            for group, group_data in self.groups.items():
                summary["nodes"].append(dict(
                    name=group,
                    data=[dict(
                        id=match.match_id,
                        participants=match.participant_ids,
                        winner=match.winner.pid
                    ) for match in group_data["matches"]]
                ))
                w, l = [int(x) for x in group.split("-")]
                prev_1 = f"{w - 1}-{l}"
                prev_2 = f"{w}-{l - 1}"
                if prev_1 in self.groups:
                    summary["edges"].append((prev_1, group))
                if prev_2 in self.groups:
                    summary["edges"].append((prev_2, group))

            return summary

        def create_initial_group(self):
            player_split = list(
                self.t.divide_chunks(sorted(self.t.roster, key=self.t.tournament_initial_matchup_strategy),
                                     self.t.players_per_match))
            for matched_players in player_split:
                ## Ensure that all players are 0-0 at the start
                assert [f"{m.wins}-{m.defeats}" for m in matched_players].count("0-0") == len(matched_players)

                self.groups["0-0"]["matches"].append(
                    Tournament.Match(
                        None,
                        self.env,
                        self.t.players_per_match,
                        self.t.best_of_n_matches,
                        self.t.tournament_strategy
                    )
                )

                self.groups["0-0"]["matches"][-1].set_match_id(self.match_counter)
                for p in matched_players:
                    self.groups["0-0"]["matches"][-1].add_participant(p)
                self.match_counter += 1

    def __init__(self,
                 env_cls,
                 players_per_match=2,
                 byes_enabled=False,
                 best_of_n_matches=3,
                 default_player_rank=500,
                 tournament_strategy=Strategy.SINGLE_ELIMINATION,
                 initial_matchup_strategy=SortStrategy.RANK
                 ):

        self.env_cls = env_cls
        self.players_per_match = players_per_match
        self.byes_enabled = byes_enabled
        self.best_of_n_matches = best_of_n_matches
        self.default_player_rank = default_player_rank
        self.tournament_is_started = False
        self.tournament_strategy = tournament_strategy
        self.tournament_initial_matchup_strategy = initial_matchup_strategy

        self.roster = []  # The full player roster

        if byes_enabled and tournament_strategy == Strategy.SWISS:
            raise RuntimeError("Byes are not supported in the SWISS Tournament Format")

        if tournament_strategy == Strategy.SINGLE_ELIMINATION:
            self.model = Tournament.SingleEliminationModel(
                tournament=self
            )
        elif tournament_strategy == Strategy.SWISS:
            self.model = Tournament.SwissModel(
                tournament=self
            )

    def reset(self):
        self.model.reset()
        for p in self.roster:
            p.reset()

    def start(self):
        return self.model.start()

    def add_agent(self, agent_cls, rank=None):
        if self.tournament_is_started:
            raise RuntimeError("Cannot add players after the tournament is started.")
        rank = rank if rank else self.default_player_rank

        self.roster.append(Tournament.PlayerInfo(
            player=agent_cls(env=None),
            rating=rank
        ))

    @staticmethod
    def is_pow_2(n):
        return (n & (n - 1)) == 0

    def divide_chunks(self, l, n):
        # looping till length l
        for i in range(0, len(l), n):
            yield l[i:i + n]
