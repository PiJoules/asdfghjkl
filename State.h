#ifndef STATE_H
#define STATE_H

/**
 * Requirements:
 * - Load game from disk
 * - Output state to screen (print state)
 * - Copy state
 */

#include <string>
#include <vector>

namespace sdp {
    class State {
        private:
            std::vector<std::vector<int>> grid_;

        public:
            State(std::vector<std::vector<int>>& grid): grid_(grid){}
            State(const std::string&);

            friend std::ostream& operator<<(std::ostream& out, const State& state);

            int** copy();
    };
}

#endif
