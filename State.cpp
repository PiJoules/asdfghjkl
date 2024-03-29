#include "State.h"
#include <vector>
#include <set>
#include <utility>
#include <cmath>
#include <algorithm>
#include <stdlib.h>

namespace sbp {
    std::vector<std::vector<int>> State::file_to_grid(const std::string& filename) const {
        std::vector<std::vector<int>> grid;
        std::ifstream state_file(filename.c_str());

        // First line - w,h,
        std::string line;
        getline(state_file, line);
        std::vector<std::string> parts = split(line, ",");
        int width = std::stoi(parts[0]);
        int height = std::stoi(parts[1]);

        // Remainder of file
        grid.resize(height, std::vector<int>(width));
        for (int y = 0; y < height; y++){
            getline(state_file, line);
            parts = split(line, ",", false);
            for (int x = 0; x < width; x++){
                grid[y][x] = std::stoi(parts[x]);
            }
        }

        return grid;
    }

    /**
     * Constructor that loads state from file.
     */
    State::State(const std::string& filename){
        grid_ = file_to_grid(filename);
    }

    /**
     * Print current state to console.
     */
    std::ostream& operator<<(std::ostream& out, const State& state){
        std::vector<std::vector<int>> grid = state.grid_;
        out << grid[0].size() << "," << grid.size() << "," << std::endl;
        for (int y = 0; y < grid.size(); y++){
            std::vector<int> row = grid[y];
            for (const auto& elem : row){
                out << elem << ",";
            }
            if (y < grid.size() - 1){
                out << std::endl;
            }
        }
        return out;
    }

    /**
     * Create clone of state.
     */
    State State::clone() const{
        State state = State(grid_);
        return state;
    }

    /**
     * Puzzle check completion.
     */
    const bool State::is_complete() const{
        for (auto& row : grid_){
            for (auto& elem : row){
                if (elem == -1){
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * List of possible moves.
     * All possible moves for a piece are:
     * - Moves where the destination is 1 shift for the piece in any 1 direction.
     * - This destination must all be 0s.
     *
     * This method exits the program if the piece is not found.
     *
     * This method assumes (does not check):
     * - The piece is also rectangular.
     */
    const std::vector<Move> State::possible_moves(const int piece) const{
        std::vector<Move> moves;

        // Invalid pieces cannot move
        if (piece < 2){
            return moves;
        }

        // Get piece location
        std::pair<uint64_t, uint64_t> position = pos(piece);
        int startx = position.first;
        int starty = position.second;

        // Exit if piece does not exist
        if (startx == -1 || starty == -1){
            std::cerr << "The piece '" << piece << "' does not exist." << std::endl;
            exit(EXIT_FAILURE);
        }

        // Get piece dimensions
        int width = 0, height = 0;
        for (int y = starty; y < grid_.size(); y++){
            if (grid_[y][startx] == piece){
                height++;
            }
            else {
                break;
            }
        }
        for (int x = startx; x < grid_[0].size(); x++){
            if (grid_[starty][x] == piece){
                width++;
            }
            else {
                break;
            }
        }

        // Check surrounding area
        // Left
        bool left_ok = true;
        if (startx > 0){
            for (int y = starty; y < starty + height; y++){
                if (grid_[y][startx-1] == -1 && piece == 2){
                    break;
                }
                else if (grid_[y][startx-1] != 0){
                    left_ok = false;
                    break;
                }
            }
        }
        if (left_ok){
            moves.push_back(Move(LEFT, piece));
        }

        // Right
        bool right_ok = true;
        if (startx + width < grid_[0].size()){
            for (int y = starty; y < starty + height; y++){
                if (grid_[y][startx + width] == -1 && piece == 2){
                    break;
                }
                else if (grid_[y][startx + width] != 0){
                    right_ok = false;
                    break;
                }
            }
        }
        if (right_ok){
            moves.push_back(Move(RIGHT, piece));
        }

        // Up
        bool up_ok = true;
        if (starty > 0){
            for (int x = startx; x < startx + width; x++){
                if (grid_[starty-1][x] == -1 && piece == 2){
                    break;
                }
                else if (grid_[starty-1][x] != 0){
                    up_ok = false;
                    break;
                }
            }
        }
        if (up_ok){
            moves.push_back(Move(UP, piece));
        }

        // Down
        bool down_ok = true;
        if (starty + height < grid_.size()){
            for (int x = startx; x < startx + width; x++){
                if (grid_[starty + height][x] == -1 && piece == 2){
                    break;
                }
                else if (grid_[starty + height][x] != 0){
                    down_ok = false;
                    break;
                }
            }
        }
        if (down_ok){
            moves.push_back(Move(DOWN, piece));
        }

        return moves;
    }

    /**
     * List of possible moves for the state.
     */
    const std::vector<Move> State::possible_moves() const{
        std::set<int> checked;
        std::vector<Move> moves;
        for (auto& row : grid_){
            for (auto& piece : row){
                if (checked.find(piece) == checked.end()){
                    std::vector<Move> piece_moves = possible_moves(piece);
                    moves.reserve(moves.size() + piece_moves.size());  // Preallocate memory
                    moves.insert(moves.end(), piece_moves.begin(), piece_moves.end());
                    checked.insert(piece);
                }
            }
        }
        return moves;
    }

    /**
     * Apply move onto the state.
     *
     * This method assumes:
     * - The move is a valid one.
     */
    void State::apply_move(const Move& move){
        // Find piece
        int piece = move.piece();
        bool found = false;
        int startx = -1, starty = -1;
        for (int y = 0; y < grid_.size() && !found; y++){
            std::vector<int> row = grid_[y];
            for (int x = 0; x < row.size(); x++){
                if (row[x] == piece){
                    startx = x;
                    starty = y;
                    found = true;
                    break;
                }
            }
        }

        // Get piece dimensions
        int width = 0, height = 0;
        for (int y = starty; y < grid_.size(); y++){
            if (grid_[y][startx] == piece){
                height++;
            }
            else {
                break;
            }
        }
        for (int x = startx; x < grid_[0].size(); x++){
            if (grid_[starty][x] == piece){
                width++;
            }
            else {
                break;
            }
        }

        // Shift pieces
        switch (move.direction()){
            case UP:
                // Set new pieces; remove old pieces
                for (int x = startx; x < startx + width; x++){
                    grid_[starty-1][x] = piece;
                    grid_[starty + height - 1][x] = 0;
                }
                break;
            case DOWN:
                for (int x = startx; x < startx + width; x++){
                    grid_[starty + height][x] = piece;
                    grid_[starty][x] = 0;
                }
                break;
            case LEFT:
                for (int y = starty; y < starty + height; y++){
                    grid_[y][startx-1] = piece;
                    grid_[y][startx + width - 1] = 0;
                }
                break;
            default:
                for (int y = starty; y < starty + height; y++){
                    grid_[y][startx + width] = piece;
                    grid_[y][startx] = 0;
                }
        }
    }

    /**
     * Apply move on cloned state.
     */
    State State::apply_move_cloning(const Move& move) const{
        State state = this->clone();
        state.apply_move(move);
        return state;
    }

    /**
     * State comparison.
     */
    const bool State::operator==(const State& other) const{
        // Compare dimensions
        if (this->grid_.size() != other.grid().size()){
            return false;
        }
        if (this->grid_[0].size() != other.grid()[0].size()){
            return false;
        }

        State clone1 = clone();
        State clone2 = other.clone();
        clone1.normalize();
        clone2.normalize();

        // Compare elements
        const auto grid1 = clone1.grid();
        const auto grid2 = clone2.grid();
        for (int y = 0; y < grid1.size(); y++){
            for (int x = 0; x < grid1[0].size(); x++){
                if (grid1[y][x] != grid2[y][x]){
                    return false;
                }
            }
        }
        return true;
    }

    const bool State::operator!=(const State& other) const{
        return !(*this == other);
    }

    /**
     * Normalize state.
     */
    void State::normalize(){
        int nextIdx = 3;
        for(int y = 0; y < grid_.size(); y++) {
            for(int x = 0; x < grid_[0].size(); x++) {
                if (grid_[y][x]==nextIdx) {
                    nextIdx++;
                }
                else if (grid_[y][x] > nextIdx) { 
                    swapIdx(nextIdx, grid_[y][x]); 
                    nextIdx++;
                }
            }
        }
    }

    /**
     * Swap pieces inplace.
     */
    void State::swapIdx(int idx1, int idx2){
        for(int y = 0; y < grid_.size(); y++) {
            for(int x = 0; x < grid_[0].size(); x++) {
                if (grid_[y][x]==idx1) {
                    grid_[y][x]=idx2;
                }
                else if (grid_[y][x]==idx2) {
                    grid_[y][x]=idx1;
                }
            }
        }
    }

    /**
     * Check if the grid contains a piece.
     */
    const bool State::contains(int piece) const {
        for (const auto& row : grid_){
            if (std::find(row.begin(), row.end(), piece) != row.end()){
                return true;
            }
        }
        return false;
    }

    /**
     * Find position of top left corner of a piece.
     * Not found if coords are (0,0). A wall is always at position (0,0).
     */
    const std::pair<int64_t, int64_t> State::pos(const int piece) const {
        std::pair<uint64_t, uint64_t> position(0, 0);
        for (auto it = grid_.begin(); it != grid_.end(); ++it){
            const auto row = *it;
            auto pos = std::find(row.begin(), row.end(), piece) - row.begin();
            if (pos < row.size()){
                position.first = pos;  // x
                position.second = it - grid_.begin();  // y
                return position;
            }
        }
        return position;
    }

    /**
     * Manhattan distance implementation.
     */
    const uint64_t State::manhattan_dist(int piece1, int piece2) const {
        // Check if both pieces are in the grid. Otherwise, return 0.
        if (!contains(piece1) || !contains(piece2)){
            return 0;
        }

        // Find distance
        const auto pos1 = pos(piece1);
        const auto pos2 = pos(piece2);
        return std::abs(pos1.first - pos2.first) + std::abs(pos1.second - pos2.second);
    }

    /**
     * Heuristic for determining how far we are from goal.
     * The default will be the manhattan distance.
     */
    const uint64_t State::heuristic() const {
        return manhattan_dist(2, -1);
    }

    /**
     * Get all possible states that can be made from the list
     * of possible moves.
     */
    const std::vector<State> State::neighbor_states() const {
        std::vector<State> neighbors;
        for (const Move& move : possible_moves()){
            neighbors.push_back(apply_move_cloning(move));
        }
        return neighbors;
    }
};
