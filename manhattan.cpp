#include <iostream>
#include "AStar.h"

using namespace std;
using namespace sbp;

int main(int argc, char* argv[]){
    sbp::State state(argv[1]);
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
