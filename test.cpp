#include <iostream>
#include <cassert>
#include <utility>
#include "State.h"
#include "Algorithms.h"

using namespace sbp;
using namespace std;

/**
 * Test clones have the same grid and are 2 different objects.
 */
void test_clone(const State& state){
    State clone = state.clone();
    assert(clone == state);

    const Move move = Move(LEFT, 2);
    clone.apply_move(move);
    assert(state != clone);
}

/**
 * Test all possible neighbor states exist.
 * I am lazy and instead will just check the number.
 */
void test_neighbor_states(const State& state){
    assert(state.neighbor_states().size() == 2);
}

void test_contains(const State& state){
    assert(state.contains(3));
    assert(!state.contains(5));
}

void test_pos(const State& state){
    assert(state.pos(2) == (pair<uint64_t, uint64_t>(2, 2)));
    assert(state.pos(-1) == (pair<uint64_t, uint64_t>(1, 0)));
    assert(state.pos(5) == (pair<uint64_t, uint64_t>(0, 0)));
}

void test_manhattan_dist(const State& state){
    assert(state.manhattan_dist(2, -1) == 3);  // Existing piece
    assert(state.manhattan_dist(5, -1) == 0);  // Nonexistant piece
}

int main(int argc, char* argv[]){
    State state("SBP-test.txt");

    cout << "Testing clone..." << endl;
    test_clone(state);

    cout << "Testing constains piece..." << endl;
    test_contains(state);

    cout << "Testing piece position..." << endl;
    test_pos(state);

    cout << "Testing manhattan distance..." << endl;
    test_manhattan_dist(state);

    cout << "Testing neighbor states..." << endl;
    test_neighbor_states(state);

    cout << "Tests passed!" << endl;
    return 0;
}

