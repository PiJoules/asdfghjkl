.PHONY: default
default: run;

CPP_FILES = State.cpp Algorithms.cpp AStar.cpp
STATE ?= states/SBP-level0.txt
OUTPUT = output-part3.txt

%.o: %.cpp
	g++ $< $(CPP_FILES) -std=c++11 -o $@

%: %.o
	./$< $(STATE)

build: bfs.o dfs.o ids.o manhattan.o blockage.o

run: manhattan.o blockage.o
	echo "A* (Manhattan Distance)" > $(OUTPUT)
	{ time ./manhattan.o states/SBP-level0.txt; echo ""; } >>$(OUTPUT) 2>&1
	{ time ./manhattan.o states/SBP-level1.txt; echo ""; } >>$(OUTPUT) 2>&1
	{ time ./manhattan.o states/SBP-level2.txt; echo ""; } >>$(OUTPUT) 2>&1
	{ time ./manhattan.o states/SBP-level3.txt; echo ""; } >>$(OUTPUT) 2>&1
	echo "A* (Blockage)" >> $(OUTPUT)
	{ time ./blockage.o states/SBP-level0.txt; echo ""; } >>$(OUTPUT) 2>&1
	{ time ./blockage.o states/SBP-level1.txt; echo ""; } >>$(OUTPUT) 2>&1
	{ time ./blockage.o states/SBP-level2.txt; echo ""; } >>$(OUTPUT) 2>&1
	{ time ./blockage.o states/SBP-level3.txt; echo ""; } >>$(OUTPUT) 2>&1

clean:
	rm -f *.o

test.o:
	g++ test.cpp $(CPP_FILES) -std=c++11 -o $@
	./$@

