




if __name__ == "__main__":
    x = Scenario(
        python.Game("15x15-2v2.json", n_players=2),
        Scenario.GOLD_COLLECT(1),
        Scenario.OIL_COLLECT(1),
        Scenario.LUMBER_COLLECT(1),
        Scenario.FOOD_CONSUMPTION(1),
        Scenario.FOOD_COUNT(1),
        Scenario.DAMAGE_DONE(1),
        Scenario.DAMAGE_TAKEN(1),
        Scenario.UNITS_CREATED(1),
        Scenario.NUM_FOOTMAN(1),
        Scenario.NUM_PEASANT(1),
        Scenario.NUM_ARCHER(1),
        Scenario.NUM_FARM(1),
        Scenario.NUM_BARRACKS(1),
        Scenario.NUM_TOWN_HALL(1)
    )

    print(x.evaluate())
