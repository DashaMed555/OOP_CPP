#include <iostream>
#include <string>
#include "parser.h"
#include "wav_processing.h"
#include "converters.h"

void help() {
    std::cout << "--------------------------- Usage ---------------------------" << std::endl;
    std::cout << "The program should run like this:" << std::endl;
    std::cout << "> sound_processor [-h] [-c config.txt output.wav input1.wav [input2.wav ...]]" << std::endl;
    std::cout << "--------------------------- Options ---------------------------" << std::endl;
    std::cout << "-h - help" << std::endl;
    std::cout << "-c - work with files .wav format" << std::endl;
    std::cout << "--------------------------- Parameters ---------------------------" << std::endl;
    std::cout << "1 - a file with configurations for processing .wav files" << std::endl;
    std::cout << "2 - the name for an output file which will contain the result of processing .wav files" << std::endl;
    std::cout << "3.. - a sequence of .wav files which can be used for processing" << std::endl;
    std::cout << "--------------------------- Supported converters ---------------------------" << std::endl;
    std::cout << "The list of the converters: | ";
    std::vector<Converter*> supported_converters = get_supported_converters();
    for (const auto& converter_info : supported_converters)
        std::cout << converter_info->get_name() << " | ";
    std::cout << std::endl << "The details: "<< std::endl;
    for (const auto& converter_info : supported_converters) {
        std::cout << converter_info->get_name() << ":\n\t" << converter_info->get_description() << std::endl;
        std::cout << "Parameters: " << std::endl;
        for (const auto& parameter : converter_info->get_parameters())
            std::cout << "\t" << parameter << std::endl;
        std::cout << "Syntax: " << converter_info->get_syntax() << std::endl << std::endl;
    }
}

std::vector<std::ifstream>* parameters_preprocessing(std::vector<std::string>& parameters, std::vector<std::string> wavs) {
    auto streams = new std::vector<std::ifstream>;
    for (auto& parameter : parameters)
        if (parameter[0] == '$') {
            parameter = wavs[std::stoi(parameter.substr(1)) + 1];
            if (check_title(parameter))
                streams->push_back(get_samples_stream(parameter));
        }
    return streams;
}

int main(int argc, char* argv[]) {
    Cmd_Parser cmd_parser;
    cmd_parser.parse_cmd(argc, argv);
    if (cmd_parser.get_option() == "-h")
        help();
    else if (cmd_parser.get_option() == "-c") {
        if (cmd_parser.get_parameters().size() < 3) {
            std::cerr << "There's no enough parameters." << std::endl;
            help();
        }
        Configs_Parser configs_parser;
        configs_parser.parse_configs(cmd_parser.get_parameters()[0]);
        auto parameters = configs_parser.get_parameters();
        std::string stream_name = cmd_parser.get_parameters()[2];
        std::string output_name = "tmp1";
        for (size_t i = 0; i < configs_parser.get_commands().size(); ++i) {
            Factory* factory = create_factory(configs_parser.get_commands()[i]);
            if (factory == nullptr)
                std::cerr << "Invalid configs file" << std::endl;
            std::vector<std::ifstream>* streams = parameters_preprocessing(parameters[i], cmd_parser.get_parameters());
            Converter* converter = factory->create_converter(parameters[i], *streams);
            std::ifstream stream;
            if (stream_name == cmd_parser.get_parameters()[2]) {
                if (not check_title(stream_name)) {
                    std::cerr << "Not supported format for " << stream_name << " " << std::endl;
                    return 1;
                }
                stream = get_samples_stream(stream_name);
            }
            else
                stream.open(stream_name);
            converter->process(stream, output_name);
            stream_name = output_name;
            output_name = output_name == "tmp1" ? "tmp2" : "tmp1";
        }
        convert_to_wav(stream_name, cmd_parser.get_parameters()[1]);
    }
    else {
        std::cerr << "Invalid command parameters" << std::endl;
        return 1;
    }
    remove("tmp1");
    remove("tmp2");
    return 0;
}
