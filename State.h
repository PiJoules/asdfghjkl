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

namespace sdp {
    enum Direction {UP, DOWN, LEFT, RIGHT};

    class Move {
        private:
            Direction dir_;
            int piece_;

        public:
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
    };

    class State {
        private:
            std::vector<std::vector<int>> grid_;
            void swapIdx(int idx1, int idx2);

        public:
            State(std::vector<std::vector<int>> grid): grid_(grid){}
            // Constructor for loading game from filename (disk)
            State(const std::string&);

            // Getter methods
            const std::vector<std::vector<int>>& grid() const{
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
    };
}

#endif
