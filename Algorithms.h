#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "State.h"

namespace sbp {
    void random_walk(State& state, const int& n);
    State bfs(const State state);
    State dfs(const State state, std::vector<State> visited, MoveTracker mt, size_t explored_nodes);
    State ids(const State state);
}

#endif
