#ifndef ASTAR_H
#define ASTAR_H

#include "State.h"

namespace sbp {
    const std::vector<Move> A_star(const State start);
    class SingleGoalState : public State {
        public:
            SingleGoalState(){}
            SingleGoalState(const std::string& filename):State(filename){}
            const uint64_t heuristic() const;
    };
}

#endif
