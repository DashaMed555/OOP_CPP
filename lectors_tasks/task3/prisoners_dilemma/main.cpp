#include <iostream>
#include "dilemma.h"
#include "parser.h"
#include "tests.cpp"

void help() {
    std::cout << "\tCommand line parameters:" << std::endl;
    std::cout << "Required: 3 strategy names" << std::endl;
    std::cout << "Optional: --mode=[detailed|fast|tournament]" << std::endl;
    std::cout << "Optional: --steps=<n>" << std::endl;
    std::cout << "Optional: --configs=<dirname>" << std::endl;
    std::cout << "Optional: --matrix=<filename>" << std::endl;
}

int main(int argc, char* argv[]) {
    //testing::InitGoogleTest();
    if (argc < 4) {
        std::cerr << "Not enough parameters. Minimum: 3 prisoners names" << std::endl;
        help();
        return 1;
    }
    State state;
    parse_cmd_args(argc, argv, state);
    Simulator policeman(state.names, state.configs, state.matrix, state.steps, state.mode);
    policeman.start();
    return 0;
    return RUN_ALL_TESTS();
}

// ./dilemma all_c all_d random --mode=detailed --steps=5 --configs=configs --matrix=matrix.txt
// all_c
// all_d
// random

// win_stay
// meta
// revenge
