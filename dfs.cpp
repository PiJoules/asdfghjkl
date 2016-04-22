#include "State.h"
#include "Algorithms.h"
#include <string>
#include <vector>

using namespace sbp;

int main(int argc, char* argv[]){
    State state(argv[1]);
    std::cout << dfs(state, std::vector<State>(), MoveTracker(state), 0) << std::endl;
    return 0;
}

