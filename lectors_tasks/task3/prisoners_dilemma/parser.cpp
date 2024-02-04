#include <iostream>
#include <fstream>

#include "parser.h"

extern std::set<std::string> supported_strategies;

void parse_mode(char& mode, std::string& parameter) {
    if (parameter == "detailed")
        mode = DETAILED;
    else if (parameter == "fast")
        mode = FAST;
    else if (parameter == "tournament")
        mode = TOURNAMENT;
}

void parse_steps(long& steps, std::string& parameter) {
    long check = std::stoi(parameter);
    if (check > 0)
        steps = check;
}

void parse_configs(std::string& directory, std::string& parameter) {
    directory = std::move(parameter);
}

void parse_matrix(std::array<std::array<double, 3>, 8>& matrix, std::string& parameter) {
    std::ifstream stream;
    stream.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    try {
        stream.open(parameter + ".txt");
        for (int row = 0; row < 8 && !stream.eof(); ++row) {
            std::string line;
            std::getline(stream, line);
            std::size_t total_pos = 0;
            for (int column = 0; column < 3; ++column) {
                std::size_t pos = 0;
                matrix[row][column] = std::stod(line.substr(total_pos), &pos);
                total_pos += pos;
            }
        }
        stream.close();
    }
    catch (const std::ifstream::failure& e) {
        std::cerr << "Caught an ifstream::failure." << std::endl;
        std::cerr << "Explanatory string: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.code() << std::endl;
    }
}

void parse_cmd_args(int argc, char* argv[], State& state) {
    for (int i = 1; i < argc; ++i) {
        std::string parameter(argv[i]);
        if (parameter.compare(0, 2, "--") == 0) {
            if (parameter.compare(2, 5, "mode=") == 0) {
                std::string mode = parameter.substr(7);
                parse_mode(state.mode, mode);
            } else if (parameter.compare(2, 6, "steps=") == 0) {
                std::string steps = parameter.substr(8);
                parse_steps(state.steps, steps);
            } else if (parameter.compare(2, 8, "configs=") == 0) {
                std::string configs = parameter.substr(10);
                parse_configs(state.configs, configs);
            } else if (parameter.compare(2, 7, "matrix=") == 0) {
                std::string matrix = parameter.substr(9);
                parse_matrix(state.matrix, matrix);
            }
        } else if (supported_strategies.count(parameter))
            state.names.push_back(parameter);
        else
            std::cerr << "Unfortunately, " << parameter << " isn't supported now." << std::endl;
    }
}

void parse_initial_decision(std::ifstream& stream, char& initial_decision) {
    std::string line;
    if (!stream.eof()) {
        stream >> line;
        if (line.compare(0, 21, "\t- initial_decision: ") == 0) {
            if (line.compare(22, line.length(), "cooperate") == 0)
                initial_decision = COOPERATE;
            else if (line.compare(22, line.length(), "defect") == 0)
                initial_decision = DEFECT;
        }
    }
}

void parse_condition_and_decision(std::ifstream& stream, char& condition, char& decision) {
    std::string line;
    if (!stream.eof()) {
        stream >> line;
        if (line.compare(0, 13, "\t\tcooperate: ") == 0) {
            condition = COOPERATE;
            line.erase(0, 13);
        }
        else if (line.compare(0, 17, "\t\tcooperate_all: ") == 0) {
            condition = COOPERATE_ALL;
            line.erase(0, 17);
        }
        else if (line.compare(0, 10, "\t\tdefect: ") == 0) {
            condition = DEFECT;
            line.erase(0, 10);
        }
        else if (line.compare(0, 14, "\t\tdefect_all: ") == 0) {
            condition = DEFECT_ALL;
            line.erase(0, 14);
        }
        else if (line.compare(0, 20, "\t\tsuccessful_round: ") == 0) {
            condition = SUCCESSFUL_ROUND;
            condition = SUCCESSFUL_ROUND;
            line.erase(0, 20);
        }
        else if (line.compare(0, 17, "\t\tneutral_round: ") == 0) {
            condition = NEUTRAL_ROUND;
            line.erase(0, 17);
        }
        else if (line.compare(0, 22, "\t\tunsuccessful_round: ") == 0) {
            condition = UNSUCCESSFUL_ROUND;
            line.erase(0, 22);
        }
        if (line.compare(0, line.size(), "cooperate") == 0)
            decision = COOPERATE;
        else if (line.compare(0, line.size(), "defect") == 0)
            decision = DEFECT;
        else if (line.compare(0, line.size(), "random") == 0)
            decision = RANDOM;
        else if (line.compare(0, line.size(), "repeat") == 0)
            decision = REPEAT;
        else if (line.compare(0, line.size(), "switch") == 0)
            decision = SWITCH;
    }
}

void parse_behavior(std::ifstream& stream, std::vector<Behavior>& behavior) {
    std::string line;
    if (!stream.eof()) {
        stream >> line;
        if (line == "\t  behavior:") {
            while (!stream.eof()) {
                stream >> line;
                char condition;
                char decision;
                parse_condition_and_decision(stream, condition, decision);
                behavior.push_back({condition, decision});
            }
        }
    }
}

void parse_name(std::ifstream& stream, std::string& name) {
    std::string line;
    if (!stream.eof()) {
        stream >> line;
        if (line.compare(0, 10, "\t\t- name: ") == 0)
            name = line.substr(10);
    }
}

void parse_probability(std::ifstream& stream, double& probability) {
    std::string line;
    if (!stream.eof()) {
        stream >> line;
        if (line.compare(0, 17, "\t\t  probability: ") == 0)
            probability = std::stod(line.substr(17));
    }
}

void parse_names_and_probabilities(std::ifstream& stream, std::vector<Strategies>& strategies) {
    std::string line;
    if (!stream.eof()) {
        stream >> line;
        if (line.compare(0, 14, "\t- strategies:") == 0) {
            std::string name;
            double probability;
            parse_name(stream, name);
            parse_probability(stream, probability);
            strategies.push_back({name, probability});
        }
    }
}

void parse_strategies(std::ifstream& stream, std::map<std::string, std::vector<Behavior>>& behavior,
                      std::map<std::string, char>& initial_decision) {
    std::string line;
    if (!stream.eof()) {
        stream >> line;
        if (line.compare(0, 11, "strategies:") == 0) {
            while (!stream.eof()) {
                stream >> line;

                std::string name;
                if (line.compare(0, 9, "\t- name: ") == 0) {
                    name = line.substr(9);
                }

                char initial_decision_;
                parse_initial_decision(stream, initial_decision_);

                std::vector<Behavior> behavior_;
                parse_behavior(stream, behavior_);

                initial_decision.insert({name, initial_decision_});
                behavior.insert({name, behavior_});
            }
        }
    }
}

void parse_strategy(std::ifstream& stream, char& initial_decision, std::vector<Behavior> behavior) {
    parse_initial_decision(stream, initial_decision);
    parse_behavior(stream, behavior);
}

void parse_meta_strategy(std::ifstream& stream, std::vector<Strategies>& strategies,
                         std::map<std::string, char>& initial_decision,
                         std::map<std::string, std::vector<Behavior>>& behavior) {
    parse_names_and_probabilities(stream, strategies);
    parse_strategies(stream, behavior, initial_decision);
}
