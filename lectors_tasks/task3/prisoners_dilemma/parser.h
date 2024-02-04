#ifndef PRISONERS_DILEMMA_PARSER_H
#define PRISONERS_DILEMMA_PARSER_H

#include <map>

#include "definitions.h"

typedef struct State {
    std::vector<std::string> names;
    char mode = DETAILED;
    long steps = 3;
    std::string configs = "configs";
    std::array<std::array<double, 3>, 8> matrix = {
            std::array<double, 3>{7, 7, 7},
            std::array<double, 3>{3, 3, 9},
            std::array<double, 3>{3, 9, 3},
            std::array<double, 3>{9, 3, 3},
            std::array<double, 3>{0, 5, 5},
            std::array<double, 3>{5, 0, 5},
            std::array<double, 3>{5, 5, 0},
            std::array<double, 3>{1, 1, 1}};;
} State;

void parse_cmd_args(int argc, char* argv[], State& state);
void parse_strategy(std::ifstream& stream, char& initial_decision, std::vector<Behavior> behavior);
void parse_meta_strategy(std::ifstream& stream, std::vector<Strategies>& strategies,
                         std::map<std::string, char>& initial_decision,
                         std::map<std::string, std::vector<Behavior>>& behavior);

#endif
