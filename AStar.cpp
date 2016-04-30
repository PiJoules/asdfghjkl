#include <unordered_set>
#include <unordered_map>
#include <limits>
#include "State.h"

namespace std {
    template <>
    struct hash<sbp::State> {
        /**
         * Hashing algorithm borrowed from python's tuple hash.
         * Here we just go through all the elements in the 2D grid.
         */
        size_t operator()(const sbp::State& state) const {
            const auto& grid = state.grid();
            if (grid.size() == 0){
                return 0;
            }

            size_t state_hash = 0x345678L;
            size_t mult = 1000003L;
            size_t len = grid.size() * grid[0].size();
            for (const auto& row : grid){
                for (const auto& elem : row){
                    len--;
                    state_hash = (state_hash ^ elem) * mult;
                    mult += 82520L + 2*len;  // No idea what this constant is
                }
            }
            state_hash += 97531L;  // No idea what this constant is
            return state_hash;
        }
    };
}

namespace sbp {
    void A_star(const State start){
        // The set of nodes already evaluated.
        std::unordered_set<State> closed_set;

        // The set of currently discovered nodes still to be evaluated.
        // Initially, only the start node is known.
        std::unordered_set<State> open_set({start});

        // For each node, which node it can most efficiently be reached from.
        // If a node can be reached from many nodes, came_from will eventually contain the
        // most efficient previous step.
        std::unordered_map<State, State> came_from;

        // For each node, the cost of getting from the start node to that node.
        // The cost of going from start to start is zero.
        std::unordered_map<State, uint64_t> g_score({{start, 0}});

        // For each node, the total cost of getting from the start node to the goal
        // by passing by that node. That value is partly known, partly heuristic.
        // For the first node, that value is completely heuristic.
        std::unordered_map<State, uint64_t> f_score({{start, start.heuristic()}});
    }
}

int main(int argc, char* argv[]){
    sbp::State state(argv[1]);
    sbp::A_star(state);
    //std::hash<sbp::State> state_hash;
    //std::cout << state_hash(state) << std::endl;
    //std::cout << state << std::endl;
    //const sbp::State cloned = state.apply_move_cloning(sbp::Move(sbp::LEFT, 2));
    //std::cout << state_hash(cloned) << std::endl;
    //std::cout << cloned << std::endl;
    //const sbp::State cloned2 = cloned.apply_move_cloning(sbp::Move(sbp::RIGHT, 2));
    //std::cout << state_hash(cloned2) << std::endl;
    //std::cout << cloned2 << std::endl;
    return 0;
}
