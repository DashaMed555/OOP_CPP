#include <fstream>
#include <iostream>
#include <cmath>
#include "converters.h"

extern union Samples {
    int16_t int16[44100];
    char chars[88200];
};

const std::string Mute_Converter::get_name() const {
    return "Mute_Converter";
}

const std::string Mute_Converter::get_description() const {
    return "Mutes the specified interval";
}

const std::vector<std::string> Mute_Converter::get_parameters() const {
    return {"start - the beginning of the interval (in seconds)",
            "end - the ending of the interval (in seconds)"};
}

const std::string Mute_Converter::get_syntax() const {
    return "mute start end";
}

Mute_Converter::Mute_Converter(const uint64_t start, const uint64_t end) {
    this->start = start;
    this->end = end;
}

void Mute_Converter::process(std::ifstream& stream, std::string output_name) {
    try {
        std::ofstream output(output_name, std::ofstream::binary);
        uint64_t seconds = 0;
        while (not stream.eof()) {
            Samples sample;
            stream.read(sample.chars, sizeof(Samples));
            if (this->start <= seconds && seconds <= this->end)
                for (auto &second: sample.int16)
                    second = 0;
            output.write(sample.chars, sizeof(Samples));
            seconds++;
        }
    }
    catch (std::ofstream::failure& e) {
        std::cerr << "Caught a failure: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.code() << std::endl;
    }
    stream.close();
}

const std::string Mix_Converter::get_name() const {
    return "Mix_Converter";
}

const std::string Mix_Converter::get_description() const {
    return "Mixes one stream with another";
}

const std::vector<std::string> Mix_Converter::get_parameters() const {
    return {"add - an additional stream",
            "position - the place of insertion (in seconds, 0 by default)"};
}

const std::string Mix_Converter::get_syntax() const {
    return "mix add position";
}

Mix_Converter::Mix_Converter(std::ifstream& add, uint64_t position = 0) {
    std::swap(this->add, add);
    this->position = position;
}

void Mix_Converter::process(std::ifstream& stream, std::string output_name) {
    try {
        std::ofstream output(output_name, std::ofstream::binary);
        uint64_t seconds = 0;
        while (not stream.eof()) {
            Samples sample;
            stream.read(sample.chars, sizeof(Samples));
            if (seconds < this->position)
                output.write(sample.chars, sizeof(Samples));
            else {
                if (not this->add.eof()) {
                    Samples sample_a;
                    this->add.read(sample_a.chars, sizeof(Samples));
                    for (int i = 0; i < 44100; ++i)
                        sample.int16[i] = (sample.int16[i] + sample_a.int16[i]) / 2;
                    output.write(sample.chars, sizeof(Samples));
                }
                else {
                    Samples sample_a;
                    this->add.read(sample_a.chars, sizeof(Samples));
                    output.write(sample.chars, sizeof(Samples));
                    break;
                }
            }
            seconds++;
        }
    }
    catch (std::ofstream::failure& e) {
        std::cerr << "Caught a failure: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.code() << std::endl;
    }
    stream.close();
}

const std::string Volume_Converter::get_name() const {
    return "Volume_Converter";
}

const std::string Volume_Converter::get_description() const {
    return "Increases and decreases the volume with a specified period";
}

const std::vector<std::string> Volume_Converter::get_parameters() const {
    return {"period - sound decrease/increase period"};
}

const std::string Volume_Converter::get_syntax() const {
    return "volume period";
}

Volume_Converter::Volume_Converter(uint64_t period) {
    this->period = period;
}

void Volume_Converter::process(std::ifstream& stream, std::string output_name) {
    try {
        std::ofstream output(output_name, std::ofstream::binary);
        uint64_t seconds = 0;
        while (not stream.eof()) {
            Samples sample;
            stream.read(sample.chars, sizeof(Samples));
            for (uint i = 0; i < 44100; ++i) {
                double k = 1.5 + sin(seconds + (double)i / 44100);
                sample.int16[i] *= k;
            }
            output.write(sample.chars, sizeof(Samples));
            seconds++;
        }
    }
    catch (std::ofstream::failure& e) {
        std::cerr << "Caught a failure: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.code() << std::endl;
    }
    stream.close();
}

Converter* Factory_Mute_Converter::create_converter(const std::vector<std::string>& parameters, std::vector<std::ifstream>& streams) const {
    return new Mute_Converter(std::stoull(parameters[0]), std::stoull(parameters[1]));
}

Converter* Factory_Mix_Converter::create_converter(const std::vector<std::string>& parameters, std::vector<std::ifstream>& streams) const {
    return new Mix_Converter(streams[0], std::stoull(parameters[1]));
}

Converter* Factory_Volume_Converter::create_converter(const std::vector<std::string>& parameters, std::vector<std::ifstream>& streams) const {
    return new Volume_Converter(std::stoull(parameters[0]));
}

std::vector<Converter*> get_supported_converters() {
    std::vector<Converter*> supported_converters;
    supported_converters.push_back(new Mute_Converter(0, 0));
    std::ifstream s;
    supported_converters.push_back(new Mix_Converter(s));
    supported_converters.push_back(new Volume_Converter(0));
    return supported_converters;
}

Factory* create_factory(const std::string& name) {
    if (name == "mute")
        return new Factory_Mute_Converter;
    if (name == "mix")
        return new Factory_Mix_Converter;
    if (name == "volume")
        return new Factory_Volume_Converter;
    return nullptr;
}
