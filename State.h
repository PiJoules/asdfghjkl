#ifndef STATE_H
#define STATE_H

/**
 * Requirements:
 *
 * 2A
 * - Load game from disk
 * - Output state to screen (print state)
 * - Copy state
 *
 * 2B
 * - Puzzle check completion
 *
 * 2C
 * - List of possible moves for give piece
 * - List of all possible moves for given board state
 * - Preform move on state
 * - Clone state, then apply move on the new state
 *
 * 2D
 * - State comparison
 *
 * 2E
 * - Normalize state
 *
 * 2F
 * - Perform random walk
 */

#include <string>
#include <vector>
#include <iostream>

namespace sbp {
    enum Direction {UP, DOWN, LEFT, RIGHT};

    class Move {
        private:
            Direction dir_;
            int piece_;

        public:
            Move(){}
            Move(Direction d, int p): dir_(d), piece_(p){}

            // Standard getter methods
            const Direction& direction() const{
                return dir_;
            }
            const int& piece() const{
                return piece_;
            }

            friend std::ostream& operator<<(std::ostream& out, const Move& move){
                std::string dir;
                switch (move.dir_){
                    case UP: dir = "UP"; break;
                    case DOWN: dir = "DOWN"; break;
                    case RIGHT: dir = "RIGHT"; break;
                    default: dir = "LEFT";
                } 
                out << "Move {direction: " << dir << ", piece: " << move.piece_
                    << "}";
                return out;
            }

            const bool operator==(const Move& other) const {
                return piece_ == other.piece() && dir_ == other.direction();
            }

            const bool operator!=(const Move& other) const{
                return !(*this == other);
            }

    };

    class State {
        private:
            std::vector<std::vector<int>> grid_;
            void swapIdx(int idx1, int idx2);

        public:
            State(){}
            State(std::vector<std::vector<int>> grid): grid_(grid){}
            // Constructor for loading game from filename (disk)
            State(const std::string&);

            // Getter methods
            std::vector<std::vector<int>> grid() const{
                return grid_;
            }

            // Printing state
            friend std::ostream& operator<<(std::ostream& out, const State& state);

            // Copying state
            State clone() const;

            // Puzzle check completion
            const bool is_complete() const;

            // List of possible moves
            const std::vector<Move> possible_moves(const int& piece) const;
            const std::vector<Move> possible_moves() const;

            // Apply moves (not given in camel case for consistency)
            void apply_move(const Move& move);
            State apply_move_cloning(const Move& move) const;

            // State comparison
            const bool operator==(const State& other) const;
            const bool operator!=(const State& other) const;

            // Normalize state
            void normalize();

            // Perform random walk
            void random_walk(const int n);

            // Grid contains piece
            const bool contains(int piece) const;

            // Position of piece.
            const std::pair<uint64_t, uint64_t> pos(int piece) const;

            // Manhattan distance
            const uint64_t manhattan_dist(int piece1, int piece2) const;

            // Default heuristic
            virtual const uint64_t heuristic() const;

            // List of possible neighbor states
            const std::vector<State> neighbor_states() const;
    };

    /**
     * Container for keeping track of move history.
     */
    class MoveTracker {
        private:
            State state_;  // Current state as result of last move;
            std::vector<Move> moves_;  // Move history

        public:
            MoveTracker(State state): state_(state){}
            MoveTracker(State state, std::vector<Move> moves): state_(state), moves_(moves){}

            State state() const{
                return state_;
            }

            std::vector<Move> moves() const {
                return moves_;
            }

            void setState(const State state){
                state_ = state;
            }

            void addMove(const Move move){
                moves_.push_back(move);
            }

            MoveTracker addMoveCloning(const Move move){
                MoveTracker mt = MoveTracker(state(), moves());
                mt.addMove(move);
                return mt;
            }

            const bool operator==(const MoveTracker& mt) const {
                return state_ == mt.state() && moves_ == mt.moves();
            }

            const bool operator!=(const MoveTracker& other) const{
                return !(*this == other);
            }
    };
}

#endif
