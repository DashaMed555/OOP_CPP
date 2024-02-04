#include <iostream>
#include <map>

template <class T, T def>
class Matrix {
public:
    Matrix() : mapping(), def_value(def){};
    int size() {
        clean();
        return mapping.size();
    }

    T& operator()(std::size_t row, std::size_t column) {
        std::pair<std::size_t, std::size_t> location = std::make_pair(row, column);
        auto it = mapping.find(location);
        if (it == mapping.end()) {
            mapping.insert({location, def_value});
            return mapping.at({std::make_pair(row, column)});
        }
        return it->second;
    }

    const std::map<std::pair<std::size_t, std::size_t>, T>& get_elements() {
        clean();
        return mapping;
    }

private:
    void clean() {
        for (auto it = mapping.begin(); it != mapping.end();) {
            if (it->second == def)
                it = mapping.erase(it);
            else
                it++;
        }
    }
    std::map<std::pair<std::size_t, std::size_t>, T> mapping;
    T def_value;
};

int main() {
    Matrix<int, -1> matrix;
    assert(matrix.size() == 0);
    auto a = matrix(0, 0);
    assert(a == -1);
    assert(matrix.size() == 0);
    matrix(100, 100) = 314;
    assert(matrix(100, 100) == 314);
    assert(matrix.size() == 1);

    Matrix<int, 0> matrix1;
    const std::size_t N = 10;
    for (std::size_t i = 0; i < N; ++i) {
        matrix1(i, i) = i;
        matrix1(N - 1 - i, i) = i;
    }

    bool flag = false;
    for (std::size_t i = 0; i < N; i++) {
        for (std::size_t j = flag; j < N; j += 2)
            matrix1(i, j) = 0;
        flag = not flag;
    }

    for (std::size_t i = 1; i < 8; i++) {
        for (std::size_t j = 1; j < 8; j++)
            std::cout << matrix1(i, j) << " ";
        std::cout << std::endl;
    }

    std::cout << "Number of busy cells: " << matrix1.size() << std::endl;
    for (auto& cell : matrix1.get_elements()) {
        std::cout << "Cell [" << cell.first.first << ", " << cell.first.second << "] = " << cell.second << std::endl;
    }
    return 0;
}
