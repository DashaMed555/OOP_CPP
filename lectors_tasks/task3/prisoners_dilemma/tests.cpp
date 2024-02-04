#include "gtest/gtest.h"
#include "parser.h"
#include "dilemma.h"

extern void parse_mode(char& mode, std::string& parameter);
extern void parse_steps(long& steps, std::string& parameter);
extern void parse_configs(std::string& directory, std::string& parameter);
extern void parse_matrix(std::array<std::array<double, 3>, 8>& matrix, std::string& parameter);

namespace Parser_TESTS {
    namespace InternalFunctions {
        TEST(InternalFunctions, ParseMode) {
            char mode;
            std::string parameter;

            parameter = "detailed";
            parse_mode(mode, parameter);
            ASSERT_EQ(mode, DETAILED);

            parameter = "fast";
            parse_mode(mode, parameter);
            ASSERT_EQ(mode, FAST);

            parameter = "tournament";
            parse_mode(mode, parameter);
            ASSERT_EQ(mode, TOURNAMENT);
        }

        TEST(InternalFunctions, ParseSteps) {
            long steps;
            std::string parameter;

            parameter = "100";
            parse_steps(steps, parameter);
            ASSERT_EQ(steps, 100);

            parameter = "12.5";
            parse_steps(steps, parameter);
            ASSERT_EQ(steps, 12);

            parameter = "-10";
            parse_steps(steps, parameter);
            ASSERT_EQ(steps, 12);
        }

        TEST(InternalFunctions, ParseConfigs) {
            std::string directory;
            std::string parameter;

            parameter = "some_configs";
            parse_configs(directory, parameter);
            ASSERT_EQ(directory, "some_configs");
        }

        TEST(InternalFunctions, ParseMatrix) {
            std::array<std::array<double, 3>, 8> matrix;
            std::string parameter;
            std::array<std::array<double, 3>, 8> expected_matrix = {std::array<double, 3>{7, 7, 7},
                                                                    std::array<double, 3>{3, 3, 9},
                                                                    std::array<double, 3>{3, 9, 3},
                                                                    std::array<double, 3>{9, 3, 3},
                                                                    std::array<double, 3>{0, 5, 5},
                                                                    std::array<double, 3>{5, 0, 5},
                                                                    std::array<double, 3>{5, 5, 0},
                                                                    std::array<double, 3>{1, 1, 1}};
            parameter = "matrix";
            parse_matrix(matrix, parameter);
            ASSERT_EQ(matrix, expected_matrix);
        }
    }

    namespace ExternalFunctions {
        TEST(ExternalFunctions, ParseCmdArgs) {
            int argc = 6;
            char* argv[] = {(char*)std::string("dilemma").c_str(),
                            (char*)std::string("--mode=detailed").c_str(),
                            (char*)std::string("--configs=configs").c_str(),
                            (char*)std::string("all_c").c_str(),
                            (char*)std::string("random").c_str(),
                            (char*)std::string("all_d").c_str()};
            State state;
            std::array<std::array<double, 3>, 8> expected_matrix = {std::array<double, 3>{7, 7, 7},
                                                                    std::array<double, 3>{3, 3, 9},
                                                                    std::array<double, 3>{3, 9, 3},
                                                                    std::array<double, 3>{9, 3, 3},
                                                                    std::array<double, 3>{0, 5, 5},
                                                                    std::array<double, 3>{5, 0, 5},
                                                                    std::array<double, 3>{5, 5, 0},
                                                                    std::array<double, 3>{1, 1, 1}};
            parse_cmd_args(argc, argv, state);
            ASSERT_EQ(state.configs, "configs");
            ASSERT_EQ(state.matrix, expected_matrix);
            ASSERT_EQ(state.names[0], "all_c");
            ASSERT_EQ(state.names[1], "random");
            ASSERT_EQ(state.names[2], "all_d");
            ASSERT_EQ(state.mode, DETAILED);
            ASSERT_EQ(state.steps, 3);
        }
    }
}

namespace Dilemma_TESTS {
    namespace Strategies {
        TEST(Strategies, AddPoints) {
            StrategyFactory factory;
            std::string null("None");
            std::string name("all_c");
            Strategy* strategy = factory.create_strategy(null, name);
            ASSERT_EQ(strategy->get_points(), 0);
            strategy->add_points(100);
            ASSERT_EQ(strategy->get_points(), 100);
        }
    }
}

TEST(Policeman, DefineThrees) {
    StrategyFactory factory;
    std::string null("None");
    std::vector<std::string> names{"all_c", "all_d", "random"};
    std::vector<Strategy*> strategies;
    for (auto name : names)
        strategies.push_back(factory.create_strategy(null, name));
    std::vector<std::array<Strategy*, 3>> threes;
    Simulator s;
    s.define_threes(strategies, threes);
    ASSERT_EQ(threes[0][0], strategies[0]);
    ASSERT_EQ(threes[0][1], strategies[1]);
    ASSERT_EQ(threes[0][2], strategies[2]);
}
