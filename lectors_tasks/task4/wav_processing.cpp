#include <fstream>
#include <iostream>
#include "wav_processing.h"

typedef struct Title {
    char chunkId[4];
    uint32_t chunkSize;
    char format[4];
    char subchunk1Id[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
} Title;

typedef struct Chunk {
    char subchunkId[4];
    uint32_t subchunkSize;
} Chunk;

typedef union Bits {
    uint32_t ints;
    char chars[4];
} Bits;

Title* get_title(std::string& stream_name) {
    auto title = new Title;
    try {
        std::ifstream stream;
        stream.open(stream_name, std::ifstream::binary);
        stream.read((char*)title, sizeof(Title));
    }
    catch (const std::ifstream::failure& e) {
        std::cerr << "Caught a failure: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.code() << std::endl;
    }
    return title;
}

bool check_title(std::string& stream_name) {
    const Title* title = get_title(stream_name);
    if (title->chunkId[0] == 'R' && title->chunkId[1] == 'I' && title->chunkId[2] == 'F' && title->chunkId[3] == 'F' &&
            title->format[0] == 'W' && title->format[1] == 'A' && title->format[2] == 'V' && title->format[3] == 'E' &&
            title->subchunk1Id[0] == 'f' && title->subchunk1Id[1] == 'm' && title->subchunk1Id[2] == 't' &&
            title->subchunk1Size == 16 && title->audioFormat == 1 && title->numChannels == 1 &&
            title->sampleRate == 44100 && title->byteRate == 88200 && title->blockAlign == 2 && title->bitsPerSample == 16)
        return true;
    return false;
}

std::ifstream get_samples_stream(std::string& stream_name) {
    std::ifstream stream;
    try {
        stream.open(stream_name, std::ifstream::binary);
        stream.seekg(sizeof(Title));
        Chunk chunk;
        while (not stream.eof()) {
            stream.read((char*)&chunk, sizeof(Chunk));
            if (std::string(chunk.subchunkId) == "data")
                break;
            stream.seekg(chunk.subchunkSize, std::ifstream::cur);
        }
    }
    catch (const std::ifstream::failure& e) {
        std::cerr << "Caught a failure: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.code() << std::endl;
    }
    return stream;
}

void convert_to_wav(const std::string& samples_container, const std::string& output_name) {
    try {
        std::ifstream samples_stream(samples_container, std::ifstream::binary);
        samples_stream.seekg(0, std::ifstream::end);
        uint32_t size = samples_stream.tellg();
        samples_stream.seekg(0, std::ifstream::beg);
        const Title title = {
                {'R', 'I', 'F', 'F'},
                32 + size,
                {'W', 'A', 'V', 'E'},
                {'f', 'm', 't', ' '},
                16,
                1,
                1,
                44100,
                88200,
                2,
                16
        };
        std::ofstream output(output_name, std::ofstream::binary);
        output.write((char*)&title, sizeof(Title));
        output.write("data", 4);
        Chunk chunk;
        chunk.subchunkSize = title.chunkSize - 32;
        Bits bits;
        bits.ints = chunk.subchunkSize;
        output.write(bits.chars, 4);
        while (not samples_stream.eof()) {
            Samples sample;
            samples_stream.read(sample.chars, sizeof(Samples));
            output.write(sample.chars, sizeof(Samples));
        }
    }
    catch (const std::ifstream::failure& e) {
        std::cerr << "Caught a failure: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.code() << std::endl;
    }
}
