.PHONY: build

CPP_FILES = State.cpp Algorithms.cpp

build:
	g++ main.cpp $(CPP_FILES) -std=c++11 -o sbp

build_bfs:
	g++ bfs.cpp $(CPP_FILES) -std=c++11 -o sbp

build_dfs:
	g++ dfs.cpp $(CPP_FILES) -std=c++11 -o sbp

build_ids:
	g++ ids.cpp $(CPP_FILES) -std=c++11 -o sbp

test:
	g++ test.cpp $(CPP_FILES) -std=c++11 -o sbp_test
	./sbp_test

test_A_star:
	g++ $(CPP_FILES) AStar.cpp -std=c++11 -o sbp_test
	./sbp_test SBP-test.txt SBP-test-solution.txt

