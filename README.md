# Sliding Block Puzzle
This program currently provides an envoronment for runnning a sliding block puzzle for CS 380.

A profile of how many nodes are explored, how much time does the search take, and 
the length of the solution found is availble in `output-part3` for levels 0-3. The
reamining levels are not included because the algroithmntook too much time to run 
after level 3.

## Dependencies
- `make`
- `g++` compiler that supports the C++11 standard

## Usage
To build, simply run
```sh
$ make
```
This uses g++ to compile the main script with the `-std` flag set to `c++11`.

### Search Algroithms
These are the executables built that can be used to find solutions.
To run any of them, use make with the first argument as the shortened name for it
and an optional states file as the STATES flag.
By default, the STATES flag is states/SBP-level0.txt.

#### Breadth First Search
```sh
$ make bfs STATE=states/SBP-level0.txt
```

#### Depth First Search
```sh
$ make dfs STATE=states/SBP-level0.txt
```

#### Iterative Deepening Search
```sh
$ make ids STATE=states/SBP-level0.txt
```

#### A* Search
Two heuristics are provided for A*.

##### Manhattan Distance
```sh
$ make manhattan STATE=states/SBP-level0.txt
```

##### Blockage
This is the custom heuristic implemented for A*.

This heuristic works by taking the manhattan distance, and adding an extra cost if
a piece is blocking the goal. If a piece is in the way of the goal, 1 is added to 
the cost, 0 otherwise. The reasoning behind this is that we encourage states that 
have no other piece blocking the goal to be searched since they may take fewer moves
to find a solution. This heuristic is also admissible since the fewest possible number
of moves required to reach the goal is the optimal one carried over by the manhattan
distance.

```sh
$ make blockage STATE=states/SBP-level0.txt
```

##### Performance
A dry run of both A* heuristics can be performed with
```sh
$ make run
```
This will print how many nodes are explored, how much time does the search take,
and the length of the solution found in a file called `output-part3.txt`.
This file should already be provided in this repo.
The reamining levels are not included because the algroithmntook too much time to run 
after level 3.

