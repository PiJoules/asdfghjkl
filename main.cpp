#include <iostream>
#include "State.h"

using namespace sdp;

int main(int argc, char* argv[]){
    State state(argv[1]);

    std::cout << state << std::endl;

    return 0;
}

