#include <iostream>
#include <cassert>
#include "State.h"
#include "Algorithms.h"

using namespace sbp;
using namespace std;

int main(int argc, char* argv[]){
    State state(argv[1]);

    if (state.is_complete()){
        cout << "This puzzle is already complete." << endl;
        return 0;
    }

    cout << state << endl;

    cout << "State moves" << endl;
    for (auto& move : state.possible_moves()){
        cout << move << endl;
    }

    state.apply_move(Move(LEFT, 2));
    cout << state << endl;

    State state2 = state.apply_move_cloning(Move(RIGHT, 2));
    cout << "Original" << endl;
    cout << state << endl;
    cout << "Copy" << endl;
    cout << state2 << endl;

    assert(state == state.clone());
    assert(state != state2);

    state.normalize();
    cout << "Normalized" << endl;
    cout << state << endl;

    random_walk(state, 1000);

    return 0;
}

