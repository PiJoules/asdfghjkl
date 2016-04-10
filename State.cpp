#include "State.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <stdlib.h>
#include <random>
#include <iterator>

namespace sdp {
    /**
     * ===Utility functions===
     */

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
     * Generic split string function.
     * Args:
     *     keep_empty (bool): Retain empty strings in the returned vector if set to true.
     *         Defaults to true.
     */
    static std::vector<std::string> split(const std::string& s, const std::string& delim,
            const bool keep_empty=true){
        std::vector<std::string> parts;
        size_t pos = 0, last_pos = 0;
        std::string token;
        while ((pos = s.find(delim, last_pos)) != std::string::npos) {
            token = s.substr(last_pos, pos - last_pos);
            if (keep_empty || token.length() > 0){
                parts.push_back(token);
            }
            last_pos = pos + 1;
        }
        token = s.substr(last_pos);
        if (keep_empty || token.length() > 0){
            parts.push_back(token);
        }
        return parts;
    }

    /**
     * ===State===
     */

    /**
     * Constructor that loads state from file.
     */
    State::State(const std::string& filename){
        std::ifstream state_file(filename.c_str());

        // First line - w,h,
        std::string line;
        getline(state_file, line);
        std::vector<std::string> parts = split(line, ",");
        int width = std::stoi(parts[0]);
        int height = std::stoi(parts[1]);

        // Remainder of file
        grid_.resize(height, std::vector<int>(width));
        for (int y = 0; y < height; y++){
            getline(state_file, line);
            parts = split(line, ",", false);
            for (int x = 0; x < width; x++){
                grid_[y][x] = std::stoi(parts[x]);
            }
        }
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
        return State(grid_);
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
    const std::vector<Move> State::possible_moves(const int& piece) const{
        std::vector<Move> moves;

        // Invalid pieces cannot move
        if (piece < 2){
            return moves;
        }

        // Get piece location
        int startx = -1, starty = -1;
        bool found = false;
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
                if (grid_[y][startx-1] != 0){
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
                if (grid_[y][startx + width] != 0){
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
                if (grid_[starty-1][x] != 0){
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
                if (grid_[starty + height][x] != 0){
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

        // Compare elements
        for (int y = 0; y < this->grid_.size(); y++){
            for (int x = 0; x < this->grid_[0].size(); x++){
                if (this->grid_[y][x] != other.grid()[y][x]){
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
     * Perform random walk given move cap.
     * 1. Generate all possible moves for board in current state.
     * 2. Select 1 move at random.
     * 3. Execute move.
     * 4. Normalize resulting game state.
     * 5. Stop if reached goal or reached cap on moves. Otherwise, repeat at 1.
     *
     * Print the state and move after each iteration.
     */
    void State::random_walk(const int n){
        // Print initial state
        std::cout << *this << std::endl;

        std::vector<Move> moves = possible_moves();
        for (int i = 0; i < n; i++){
            Move move = *random_element(moves.begin(), moves.end());
            apply_move(move);

            // Print move and state
            std::cout << move << std::endl;
            std::cout << *this << std::endl;

            normalize();
            if (is_complete()){
                break;
            }
            moves = possible_moves();
        }
    }
};
