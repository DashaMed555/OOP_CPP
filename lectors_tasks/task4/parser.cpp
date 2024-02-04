#include <fstream>
#include <iostream>
#include "parser.h"

void Cmd_Parser::parse_cmd(int argc, char* argv[]) {
    if (argc > 1) {
        int i = 1;
        std::string option_str(argv[i]);
        if (option_str == "-h")
            this->option = option_str;
        else if (option_str == "-c") {
            this->option = option_str;
            for (++i; i < argc; ++i)
                this->parameters.emplace_back(argv[i]);
        }
    }
}

const std::string& Cmd_Parser::get_option() {
    return this->option;
}

const std::vector<std::string>& Cmd_Parser::get_parameters() {
    return this->parameters;
}

void Configs_Parser::parse_configs(const std::string& file_path) {
    std::ifstream stream;
    try {
        stream.open(file_path);
        while (not stream.eof()) {
            std::string line;
            getline(stream, line);
            if (line[0] != '#' && not line.empty()) {
                size_t pos = line.find(' ');
                this->commands.push_back(line.substr(0, pos));
                std::vector<std::string> param;
                while (not line.empty() && pos != std::string::npos) {
                    line.erase(0, pos + 1);
                    pos = line.find(' ');
                    param.push_back(line.substr(0, pos));
                }
                this->parameters.push_back(param);
            }
        }
        stream.close();
    }
    catch (const std::ifstream::failure& e) {
        std::cerr << "Caught a failure: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.code() << std::endl;
    }
}

const std::vector<std::string> &Configs_Parser::get_commands() {
    return this->commands;
}

const std::vector<std::vector<std::string>> &Configs_Parser::get_parameters() {
    return this->parameters;
}
