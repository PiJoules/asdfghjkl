# Sliding Block Puzzle
This program currently provides an envoronment for runnning a sliding block puzzle for CS 380.

## Dependencies
- `make`
- `g++` compiler that supports the C++11 standard

## Usage
To build, simply run
```sh
$ make
```
This uses g++ to compile the main script with the `-std` flag set to `c++11`.

To load a file state and execute a random walk up to N moves:
```sh
$ ./sbp path/to/state/file.txt N
```

To build for breadth first search, depth first search, or iterative deepening:
```sh
$ make build_bfs
$ make build_dfs
$ make build_ids
```

To use any of these with a state file:
```sh
$ ./sbp path/to/state/file.txt
```

## Test
This isn't so much a test, but some debug output:
```sh
$ make test
```

