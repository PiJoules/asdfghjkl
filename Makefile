.PHONY: build

CPP_FILES = State.cpp

build:
	g++ main.cpp $(CPP_FILES) -std=c++11 -o sbp

test:
	g++ test.cpp $(CPP_FILES) -std=c++11 -o test_output
	bash test.sh

