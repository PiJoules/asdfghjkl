#include "Algorithms.h"
#include <random>
#include <iterator>
#include <vector>
#include <algorithm>

namespace sbp {
    /**
     * Select random element from vector.
     */
    template<typename Iter, typename RandomGenerator>
    Iter random_element(Iter start, Iter end, RandomGenerator& g) {
        std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
        std::advance(start, dis(g));
        return start;
    }

    template<typename Iter>
    Iter random_element(Iter start, Iter end) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return random_element(start, end, gen);
    }

    /**
     * Perform random walk given move cap.
     * 1. Generate all possible moves for board in current state.
     * 2. Select 1 move at random.
     * 3. Execute move.
     * 4. Normalize resulting game state.
     * 5. Stop if reached goal or reached cap on moves. Otherwise, repeat at 1.
     *
     * Print the state and move after each iteration.
     */
    void random_walk(State& state, const int& n){
        // Print initial state
        std::cout << state << std::endl;
        if (state.is_complete()){
            return;
        }

        std::vector<Move> moves = state.possible_moves();
        for (int i = 0; i < n; i++){
            Move move = *random_element(moves.begin(), moves.end());
            state.apply_move(move);

            // Print move and state
            std::cout << move << std::endl;
            std::cout << state << std::endl;

            state.normalize();
            if (state.is_complete()){
                break;
            }
            moves = state.possible_moves();
        }
    }

    /**
     * Breadth first search.
     * Ignore sequences of moves that lead to same state.
     */
    State bfs(const State state){
        if (state.is_complete()){
            return state;
        }

        std::vector<MoveTracker> mts;
        mts.push_back(MoveTracker(state));
        std::vector<State> states;
        std::vector<State> visited;
        states.push_back(state);

        size_t explored_nodes = 0;

        while (!states.empty()){
            explored_nodes++;
            const State first = states[0];
            MoveTracker first_tracker = mts[0];
            visited.push_back(first);
            std::vector<Move> moves = first.possible_moves();
            for (auto& move : moves){
                State state_ = first.apply_move_cloning(move);
                MoveTracker mt_ = first_tracker.addMoveCloning(move);
                if (std::find(visited.begin(), visited.end(), state_) != visited.end()){
                    continue;
                }
                if (state_.is_complete()){
                    for (auto& move : mt_.moves()){
                        std::cout << move << std::endl;
                    }
                    std::cout << "explored_nodes: " << explored_nodes << std::endl;
                    return state_;
                }
                states.push_back(state_);
                mts.push_back(mt_);
            }
            states.erase(states.begin());
            std::cout << "states: " << states.size() << std::endl;
            mts.erase(mts.begin());
        }
        return state;
    }

    /**
     * Depth first search.
     */
    State dfs(const State state, std::vector<State> visited, MoveTracker mt, size_t explored_nodes){
        if (state.is_complete()){
            return state;
        }

        visited.push_back(state);
        explored_nodes++;
        std::vector<Move> moves = state.possible_moves();
        for (auto& move : moves){
            State state_ = state.apply_move_cloning(move);
            MoveTracker mt_ = mt.addMoveCloning(move);
            if (state_.is_complete()){
                for (auto& move : mt_.moves()){
                    std::cout << move << std::endl;
                }
                std::cout << "explored_nodes: " << explored_nodes << std::endl;
                return state_;
            }
            if (std::find(visited.begin(), visited.end(), state_) != visited.end()){
                continue;
            }
            std::cout << state_ << std::endl;
            return dfs(state_, visited, mt_, explored_nodes);
        }

        return state;
    }

    /**
     * Depth limited search.
     */
    State dls(const State state, std::vector<State> visited, MoveTracker mt, size_t explored_nodes, size_t depth, size_t max_depth){
        if (depth > max_depth){
            return state;
        }
        if (state.is_complete()){
            return state;
        }

        visited.push_back(state);
        std::vector<Move> moves = state.possible_moves();
        for (auto& move : moves){
            State state_ = state.apply_move_cloning(move);
            MoveTracker mt_ = mt.addMoveCloning(move);
            if (state_.is_complete()){
                for (auto& move : mt_.moves()){
                    std::cout << move << std::endl;
                }
                std::cout << "explored_nodes: " << explored_nodes << std::endl;
                return state_;
            }
            if (std::find(visited.begin(), visited.end(), state_) != visited.end()){
                continue;
            }
            std::cout << state_ << std::endl;
            return dls(state_, visited, mt_, explored_nodes + 1, depth + 1, max_depth);
        }

        return state;
    }

    /**
     * Iterative deepening.
     */
    State ids(const State state){
        if (state.is_complete()){
            return state;
        }
        size_t max_depth = 0;
        while (1){
            State state_ = dls(state, std::vector<State>(), MoveTracker(state), 0, 0, max_depth);
            max_depth++;
        }
        return state;
    }
}

