def GOLD_COLLECT(target):
    def eval_wrapper(self, player):
        t = player.statistic_gathered_gold >= target
        r = 1 if t else 0
        return t, r

    def stats_wrapper(self, player):
        t, r = eval_wrapper(self, player)
        return ["Gold Collected", player.get_id(), player.statistic_gathered_gold, target, t]

    return dict(
        eval=eval_wrapper,
        stats=stats_wrapper
    )


def GOLD_COLLECT_INCREMENT(amount, reward_success=1, reward_fail=-0.01):
    def eval_wrapper(self, player):
        diff = player.statistic_gathered_gold - self.data.previous_statistic_gathered_gold
        self.data.previous_statistic_gathered_gold = player.statistic_gathered_gold
        r = reward_success if diff > 0 else reward_fail
        t = player.statistic_gathered_gold >= amount
        return t, r

    def stats_wrapper(self, player):
        return []

    return dict(
        eval=eval_wrapper,
        stats=stats_wrapper
    )


def LUMBER_COLLECT(amount):
    def eval_wrapper(self, player):
        p = self.game.players[player]
        return p.statistic_gathered_lumber >= amount

    def stats_wrapper(self, player):
        return []

    return dict(eval=eval_wrapper,
                stats=stats_wrapper
                )


def STONE_COLLECT(amount, player=0):
    def eval_wrapper(self, player):
        p = self.game.players[player]
        return p.statistic_gathered_stone >= amount

    def stats_wrapper(self, player):
        return []

    return dict(eval=eval_wrapper,
                stats=stats_wrapper
                )


def FOOD_CONSUMPTION(amount, player=0):
    def eval_wrapper(self, player):
        p = self.game.players[player]
        return p.food_consumption >= amount

    def stats_wrapper(self, player):
        return []

    return dict(
        eval=eval_wrapper,
        stats=stats_wrapper
    )


def FOOD_COUNT(amount, player=0):
    def eval_wrapper(self, player):
        p = self.game.players[player]
        return p.food >= amount

    def stats_wrapper(self, player):
        return []

    return dict(
        eval=eval_wrapper,
        stats=stats_wrapper
    )


def DAMAGE_DONE(amount, player=0):
    def eval_wrapper(self, player):
        p = self.game.players[player]
        return p.statistic_damage_done >= amount

    def stats_wrapper(self, player):
        return []

    return dict(
        eval=eval_wrapper,
        stats=stats_wrapper
    )


def DAMAGE_TAKEN(amount, player=0):
    def eval_wrapper(self, player):
        p = self.game.players[player]
        return p.statistic_damage_taken >= amount

    def stats_wrapper(self, player):
        return []

    return dict(
        eval=eval_wrapper,
        stats=stats_wrapper
    )


def UNITS_CREATED(amount, player=0):
    def eval_wrapper(self, player):
        p = self.game.players[player]
        return p.statistic_units_created >= amount

    def stats_wrapper(self, player):
        return []

    return dict(
        eval=eval_wrapper,
        stats=stats_wrapper
    )


def NUM_FOOTMAN(amount, player=0):
    def eval_wrapper(self, player):
        p = self.game.players[player]
        return p.num_footman >= amount

    def stats_wrapper(self, player):
        return []

    return dict(
        eval=eval_wrapper,
        stats=stats_wrapper
    )


def NUM_PEASANT(amount, player=0):
    def eval_wrapper(self, player):
        p = self.game.players[player]
        return p.num_peasant >= amount

    def stats_wrapper(self, player):
        return []

    return dict(
        eval=eval_wrapper,
        stats=stats_wrapper
    )


def NUM_ARCHER(amount, player=0):
    def eval_wrapper(self, player):
        p = self.game.players[player]
        return p.num_archer >= amount

    def stats_wrapper(self, player):
        return []

    return dict(
        eval=eval_wrapper,
        stats=stats_wrapper
    )


def NUM_FARM(amount, player=0):
    def eval_wrapper(self, player):
        p = self.game.players[player]
        return p.num_farm >= amount

    def stats_wrapper(self, player):
        return []

    return dict(
        eval=eval_wrapper,
        stats=stats_wrapper
    )


def NUM_BARRACKS(amount, player=0):
    def eval_wrapper(self, player):
        p = self.game.players[player]
        return p.num_barrack >= amount

    def stats_wrapper(self, player):
        return []

    return dict(
        eval=eval_wrapper,
        stats=stats_wrapper
    )


def NUM_TOWN_HALL(amount, player=0):
    def eval_wrapper(self, player):
        p = self.game.players[player]
        return p.num_town_hall >= amount

    def stats_wrapper(self, player):
        return []

    return dict(
        eval=eval_wrapper,
        stats=stats_wrapper
    )
