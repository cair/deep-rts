# Monte Carlo Method
From Wikipedia:
> Monte Carlo methods (or Monte Carlo experiments) are a broad class of computational algorithms that rely on repeated random sampling to obtain numerical results. Their essential idea is using randomness to solve problems that might be deterministic in principle. They are often used in physical and mathematical problems and are most useful when it is difficult or impossible to use other approaches. Monte Carlo methods are mainly used in three distinct problem classes:[1] optimization, numerical integration, and generating draws from a probability distribution.

Real Time Strategy as a AI problem has a enormous state and action space, For example in a game like chess we can define the state space as:

Board: 8x8 = 64 and Pieces: 32

Which gives us  ![](https://wik92733c1bff9c70c0e5d298b4f)=1,832,624,140,942,590,534 states

This problem is highly related to
[God's Algorithm](https://en.wikipedia.org/wiki/God%27s_algorithm) which refers to any algorithm which produces a solution having the fewest possible moves, the idea being that an omniscient being would know an optimal step from any given configuration.