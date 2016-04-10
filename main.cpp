#include "State.h"
#include <string>

using namespace sdp;

int main(int argc, char* argv[]){
    State state(argv[1]);
    state.random_walk(std::stoi(argv[2]));
    return 0;
}

