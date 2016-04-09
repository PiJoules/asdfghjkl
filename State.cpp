#include "State.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

namespace sdp {
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
};
