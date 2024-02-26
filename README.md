# Strongin

Connect Four engine, derived from random forest algorithms. Named after one of the creators of the original game.

## Main goal

Project's purpose is to show whether random tree forests could improve decision making in zero-sum games. This engine's search algorithm:
* takes a whole tree (of possible game outcomes),
* generates an x amount of samples (meaning a random tree that is a subset of the whole tree),
* and averages the evaluated score within the samples to take the best decision.

It also allows the samples to be used multiple times, which realizes the concept of random tree forest training methods.
Engine's goal is not to solve the game, but to take the correct decisions only using method described above.

## Used techniques
To optimize the searching process, engine uses:
* zobrist hashing for lookup tables
* multithreading

Optimizations, that are planned for the nearest future:
* bitboards
