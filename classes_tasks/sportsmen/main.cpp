#include <iostream>
#include <list>
#include <map>
#include <fstream>
#include "profile.h"

int main() {
    std::ifstream stream;
    try {
        stream.open("big_test.txt");
        {
            LOG_DURATION("Check")
            int sportsmen_count;
            stream >> sportsmen_count;
            std::list<int> line;
            std::map<int, std::list<int>::iterator> mapper;
            for (int i = 0; i < sportsmen_count; ++i) {
                int his_num, other_num;
                stream >> his_num >> other_num;
                auto it = mapper.find(other_num);
                if (it != mapper.end()) {
                    auto new_it = line.insert(it->second, his_num);
                    mapper.insert({his_num, new_it});
                }
                else {
                    line.push_back(his_num);
                    mapper.insert({his_num, prev(line.end())});
                }
            }
            for (auto man : line)
                std::cout << man << std::endl;
        }
        stream.close();
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "Caught an ifstream::failure." << std::endl;
        std::cerr << "Explanatory string: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.code() << std::endl;
    }
    return 0;
}
