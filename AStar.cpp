#include <unordered_set>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <vector>
#include <cmath>
#include "State.h"


namespace std {
    /**
     * State hash
     * Hashing algorithm borrowed from python's tuple hash.
     * Here we just go through all the elements in the 2D grid.
     */
    template <>
    struct hash<sbp::State> {
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

    /**
     * Move hash
     * Also borrowed from python's tuple hash, treating a move as a tuple of
     * size 2.
     */
    template <>
    struct hash<sbp::Move> {
        size_t operator()(const sbp::Move& move) const {
            size_t state_hash = 0x345678;
            size_t mult = 1000003;

            state_hash = (state_hash ^ move.direction()) * mult;
            mult += 82520 + 2;
            state_hash = (state_hash ^ move.piece()) * mult;
            mult += 82520;

            state_hash += 97531;  // No idea what this constant is
            return state_hash;
        }
    };
}

namespace sbp {
    /**
     * Recreate the path taken to find the solution.
     */
    static const std::vector<Move> reconstruct_path(const std::unordered_map<State, std::pair<State, Move>>& came_from, State current){
        std::vector<Move> total_path;
        auto it = came_from.find(current);
        while (it != came_from.end()){
            auto pair_ = it->second;
            current = pair_.first;
            total_path.push_back(pair_.second);
            it = came_from.find(current);
        }
        // Flip
        std::reverse(total_path.begin(), total_path.end());
        return total_path;
    }

    /**
     * A* implementation
     */
    const std::vector<Move> A_star(const State start){
        uint64_t explored_nodes = 0;

        // The set of nodes already evaluated.
        std::unordered_set<State> closed_set;

        // The set of currently discovered nodes still to be evaluated.
        // Initially, only the start node is known.
        std::unordered_set<State> open_set({start});

        // For each node, which node it can most efficiently be reached from.
        // If a node can be reached from many nodes, came_from will eventually contain the
        // most efficient previous step.
        std::unordered_map<State, std::pair<State, Move>> came_from;

        // For each node, the cost of getting from the start node to that node.
        // The cost of going from start to start is zero.
        // States not in this have a default value of infinity.
        std::unordered_map<State, uint64_t> g_score({{start, 0}});

        // For each node, the total cost of getting from the start node to the goal
        // by passing by that node. That value is partly known, partly heuristic.
        // For the first node, that value is completely heuristic.
        // States not in this have a default value of infinity.
        std::unordered_map<State, uint64_t> f_score({{start, start.heuristic()}});

        while (!open_set.empty()){
            explored_nodes++;

            // Get node with lowest valued f_score
            const auto current = *std::min_element(open_set.begin(), open_set.end(),
                [&f_score](const State state1, const State state2){
                    if (f_score.find(state1) == f_score.end()){
                        // State1 not in f_score
                        f_score[state1] = std::numeric_limits<uint64_t>::infinity();
                    }

                    if (f_score.find(state2) == f_score.end()){
                        // State2 not in f_score
                        f_score[state2] = std::numeric_limits<uint64_t>::infinity();
                    }

                    return f_score[state1] < f_score[state2];
                });

            // Check for completeness
            if (current.is_complete()){
                std::cout << "Explored " << explored_nodes << " nodes" << std::endl;
                return reconstruct_path(came_from, current);
            }

            // Remove element and add to closed_set
            open_set.erase(current);
            closed_set.insert(current);

            // For each possible state
            for (const auto move : current.possible_moves()){
                const auto neighbor = current.apply_move_cloning(move);

                // Ignore the neighbor which is already evaluated.
                if (closed_set.find(neighbor) != closed_set.end()){
                    continue;
                }

                // The distance from start to a neighbors
                if (g_score.find(current) == g_score.end()){
                    g_score[current] = std::numeric_limits<uint64_t>::infinity();
                }
                if (g_score.find(neighbor) == g_score.end()){
                    g_score[neighbor] = std::numeric_limits<uint64_t>::infinity();
                }
                uint64_t tentative_g_score = g_score[current] + 1;

                // Discover a new node
                if (open_set.find(neighbor) == open_set.end()){
                    open_set.insert(neighbor);
                }
                else if (tentative_g_score >= g_score[neighbor]){
                    // This is not a better path
                    continue;
                }

                // This path is the best until now. Record it.
                came_from[neighbor] = std::pair<State, Move>(current, move);
                g_score[neighbor] = tentative_g_score;
                f_score[neighbor] = g_score[neighbor] + neighbor.heuristic();
            }
        }

        std::cout << "Explored " << explored_nodes << " nodes" << std::endl;
        return std::vector<Move>();
    }


    class SingleGoalState : public State {
        private:
            static const int GOAL_PIECE = 2;
            State goal_;

            // Largest piece number on board
            int piece_max_;

        public:
            SingleGoalState(){}
            SingleGoalState(const std::string& filename, const std::string& goal_filename){
                grid_ = file_to_grid(filename);
                goal_ = State(goal_filename);

                // Get smallest and largest piece numbers
                int largest = GOAL_PIECE;
                for (const auto& row : grid_){
                    for (const auto& elem : row){
                        if (elem > largest){
                            largest = elem;
                        }
                    }
                }
                piece_max_ = largest;
            }

            const uint64_t heuristic() const {
                size_t dist = manhattan_dist(2, -1);
                // Get distance for every other piece now
                for (size_t i = GOAL_PIECE + 1; i <= piece_max_; i++){
                    const auto current_pos = pos(i);
                    const auto goal_pos = goal_.pos(i);
                    dist += std::abs(current_pos.first - goal_pos.first) +
                            std::abs(current_pos.second - goal_pos.second);
                }
                return dist;
            }
    };
}

int main(int argc, char* argv[]){
    sbp::State state(argv[1]);
    //sbp::SingleGoalState state(argv[1], argv[2]);
    std::vector<sbp::Move> path = sbp::A_star(state);

    if (path.empty()){
        std::cout << "No solution found." << std::endl;
        return 1;
    }

    std::cout << "Solution path length: " << path.size() << std::endl;
    for (const auto& move : path){
        std::cout << move << std::endl;
        state.apply_move(move);
    }
    std::cout << state << std::endl;

    return 0;
}
