#ifndef TASK4_WAV_PROCESSING_H
#define TASK4_WAV_PROCESSING_H

#include <string>
#include <cstdint>
#include <fstream>

typedef union Samples {
    int16_t int16[44100];
    char chars[88200];
} Samlpes;

bool check_title(std::string& stream_name);
std::ifstream get_samples_stream(std::string& stream_name);
void convert_to_wav(const std::string& samples_container, const std::string& output_name);

#endif
