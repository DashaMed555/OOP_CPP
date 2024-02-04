#include "ini.h"
#include <fstream>

using namespace Ini;

Section& Document::AddSection(std::string name) {
    sections.insert({name, Section()});
    return sections.at(name);
}

const Section& Document::GetSection(const std::string& name) const {
    return sections.at(name);
}

size_t Document::SectionCount() const {
    return sections.size();
}

Document Ini::Load(std::istream& input) {
    Document doc;
    std::string line;
    getline(input, line);
    while (not input.eof()) {
        if (line[0] == '[' && line.back() == ']') { \
        std::string name = line.substr(1, line.length() - 2);
            Section& section = doc.AddSection(name);
            getline(input, line);
            while ((line[0] != '[' || line.back() != ']') && not line.empty()) {
                std::size_t pos = line.find('=');
                std::string category;
                std::string value;
                category = line.substr(0, pos);
                value = line.substr(pos + 1);
                section.insert({category, value});
                getline(input, line);
            }
        }
        getline(input, line);
    }
    return doc;
}
