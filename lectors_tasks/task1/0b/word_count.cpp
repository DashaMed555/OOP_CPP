#include <string>
#include <fstream>
#include <list>
#include <map>

int main(int argc, char** argv) {

    std::ifstream input_file;
    std::string infile = argv[1];
    input_file.open(infile, std::ifstream::in);

    std::list<std::string> words;
    std::string line;
    while (getline(input_file, line)) {
        std::string::iterator it_str = line.begin();
        std::string word;
        while (it_str != line.end()) {
            while (it_str != line.end() && (std::isdigit(*it_str) || std::isalpha(*it_str))) {
                word.push_back(*it_str);
                it_str++;
            }
            while (it_str != line.end() && !(std::isdigit(*it_str) || std::isalpha(*it_str)))
                it_str++;
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
    }

    input_file.close();

    int total_words_number = 0;
    std::map<std::string, int> dictionary;
    for (auto & word : words) {
        if (dictionary.count(word))
            dictionary.find(word)->second++;
        else
            dictionary.insert(std::pair<std::string, int>(word, 1));
        total_words_number++;
    }

    std::multimap<int, std::string> reversed;
    for (auto & it_dict : dictionary)
        reversed.insert(std::pair<int, std::string>(it_dict.second, it_dict.first));

    std::ofstream output_file;
    std::string outfile = argv[2];
    output_file.open(outfile, std::ofstream::out);

    for (auto it_dict = reversed.rbegin(); it_dict != reversed.rend(); ++it_dict) {
        output_file << it_dict->second.c_str();
        output_file << ",";
        output_file << std::to_string(it_dict->first);
        output_file << ",";
        output_file << std::to_string((float)it_dict->first * 100 / (float)total_words_number);
        output_file << "%" << std::endl;
    }

    output_file.close();

    return 0;

}
