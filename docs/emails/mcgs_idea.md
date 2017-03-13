# MCGS Idea

Heia!

Jeg driver å lurer på om jeg skal spisse meg litt inn på MCTS for den virker å være ganske så åpen når det kommer til research:

http://www.cameronius.com/research/mcts/about/index.html

Så langt har jeg implementert MCTS med følgende specs:
policy_selection:  random
policy_rollout:  random

Dvs at algoritmen som den er nå bare gjør tilfeldige valg o mden skal velge eksisterende node eller om den skal ekspandere en ny "branch".

Slik den står nå klarer algoritmen å finne løsninger som fører til vinn/tap og det i seg selv er jo veldig bra, men det blir ganske dypt før det skjer (typ 1000->10 000 greiner)

Det som gjør MCTS spennende for min del er backpropogation delen som drar scoren opp i treet

Nå til hva jeg tror jeg kan gjøre som er gårdig kult da:
Jeg tenker egentlig å lage en ny algoritme kalt MCGS eller også Monte Carlo Graph Search
Den skal baseres på ca det samme som MCTS, men alle noder skal på et eller annet vis hashes (Altså hashe staten, her må jeg gjøre litt research for å få det til å gå kjapt). Tanken er da at jeg skal kunne lage en directed graph som skal kutte bort mye av "drittet" som følger med MCTS. Jeg tenker jo at algoritmen etter hvert bare skal stå å kjøre og lage en mega giga graph som senere kan brukes til å gjøre action descisions.

Da kan man også bruke pathfinding på grafen for å finne "shortest path" til victory/høy score
Dette tror jeg går under reinforment learning...

I tillegg har jeg et supplementær tre. Hensikten med det treet er og samle inn statistikk for actions og lage en score på hvor bra en action er på gitt tid. Tanken er at denne spiller inn på MCGS når den skal kjøre expansion under kalkulering (Dette blir da egentlig heuristikken til MCGS)


Grunnen til at jeg tror dette er kraftig bedre for vertfall RTS spill, og det jeg vil kalle ARG (Action Reversable Games, (Jeg fant på navnet selv!))  er at veldig mange actions i RTS har en reversible:
![](http://i.imgur.com/2vhlfSG.png)
​
Og når da i tilegg det suplementære treet kan hjelpe med action selection tror jeg faktisk det kan bli bra!

Hva tror du?

Nedenfor ligger det jeg fant om open research topics

# Open Research Topics

As MCTS is a new field of study, there are many open research topics.

## Algorithmic Enhancements
Almost all of the dozens of improvements suggested for the basic algorithm require further research. See the list in the Enhancements section for more details.

## Automated Parameter Tuning
One of the simplest open questions is how to dynamically adjust search parameters such as the UCB bias term to maximum effect, and what other aspects of the search may be similarly parameterised.

## Node Expansion
Some applications benefit from expanding the search tree one node per iteration whereas other might benefit from expanding all possible nodes per iteration. There are to date no clear guidelines as to which strategy should be applied for a given situation, and whether this can be determined automatically.

## Node Reliability
It would be useful to know how many times a given node must be visited before its reward estimate is sufficently reliable, based on its situation and relative position within the search tree.

## Tree Shape Analysis
We have already conducted preliminary work into the question of whether UCT tree shape might yield some insight into the characteristics of a given game (Williams 2010).The results are encouraging.