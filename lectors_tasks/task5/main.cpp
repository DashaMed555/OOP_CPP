#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <vector>

template <std::size_t I, typename... Args>
void print_tuple(std::ostream& os, std::tuple<Args...> const& t);

char screening_l = '"';
char screening_r = '"';

template <typename Ch, typename Tr, typename... Args>
auto& operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t) {
    os << "(";
    print_tuple<sizeof...(Args) - 1, Args...>(os, t);
    os << ")";
    return os;
}

template <std::size_t I, typename... Args>
void print_tuple(std::ostream& os, std::tuple<Args...> const& t) {
    if constexpr (I == 0)
        os << screening_l << std::get<I>(t) << screening_r;
    else {
        print_tuple<I - 1, Args...>(os, t);
        os << ", " << screening_l << std::get<I>(t) << screening_r;
    }
}

typedef struct Configs {
    char row_delimiter;
    char line_delimiter;
} Configs;

template <typename... Args>
class CSVParser {
    class LegacyInputIterator {
        std::ifstream& stream;
        std::string current;
        Configs conf;
        std::size_t current_row;
        std::size_t current_line;
    public:
        LegacyInputIterator(std::ifstream& stream, std::streamoff n_skip, const Configs& conf) : stream(stream),
                conf(conf), current_row(0), current_line(n_skip + 1) {
            stream.seekg(n_skip);
            if (not stream.eof())
                std::getline(stream, current, conf.line_delimiter);
        }

        bool operator==(const LegacyInputIterator& other) const {
            return stream.eof();
        }

        std::tuple<Args...> operator*() {
            current_row = 0;
            std::vector<std::string> cells;
            std::string cell;
            for (const auto& symbol : current) {
                if (symbol == conf.row_delimiter) {
                    cells.push_back(cell);
                    cell.clear();
                }
                else
                    cell.push_back(symbol);
            }
            cells.push_back(cell);
            return std::make_tuple(std::forward<Args>(read_data<Args>(cells))...);
        }

        LegacyInputIterator& operator++() {
            if (not stream.eof()) {
                std::getline(stream, current, conf.line_delimiter);
                current_line++;
            }
            return *this;
        }
    private:
        template <typename T>
        T read_data(std::vector<std::string>& cells) {
            try {
                T data;
                std::istringstream(cells[0]) >> data;
                cells.erase(cells.begin());
                current_row++;
                return data;
            }
            catch (std::exception& ex) {
                std::cout << "There was an error! On " << current_row << " row in " << current_line << " line! " << std::endl;
                std::cout << ex.what() << std::endl;
            }
        }
    };
public:
    CSVParser(std::ifstream& stream, std::streamoff n_skip, std::string configs) : stream(stream), n_skip(n_skip) {
        try {
            std::ifstream c_stream(configs);
            std::string line;
            while (not c_stream.eof()) {
                std::getline(c_stream, line);
                if (line.substr(0, 15) == "row_delimiter: ") {
                    line.erase(0, 15);
                    conf.row_delimiter = get_char(line);
                }
                else if (line.substr(0, 16) == "line_delimiter: ") {
                    line.erase(0, 16);
                    conf.line_delimiter = get_char(line);
                }
                else if (line.substr(0, 11) == "screening: ") {
                    line.erase(0, 11);
                    screening_l = line[0];
                    screening_r = line[1];
                }
            }
        }
        catch(std::ifstream::failure& ex) {
            std::cout << ex.what() << std::endl;
            std::cout << ex.code() << std::endl;
        }
    }

    [[nodiscard]]
    LegacyInputIterator begin() const {
        return LegacyInputIterator(stream, n_skip, conf);
    }

    [[nodiscard]]
    LegacyInputIterator end() const {
        return LegacyInputIterator(stream, n_skip, conf);
    }
private:
    char get_char(const std::string& line) {
        if (line.length() == 1)
            return line[0];
        else if (line == "\\n")
            return '\n';
        return '\0';
    }

    std::ifstream& stream;
    std::streamoff n_skip;
    Configs conf;
};

int main() {
    std::ifstream file("test.csv");
    CSVParser<int, std::string> parser(file, 0, "configs.txt");
    for (std::tuple<int, std::string> rs : parser) {
        std::cout << rs << std::endl;
    }
}
