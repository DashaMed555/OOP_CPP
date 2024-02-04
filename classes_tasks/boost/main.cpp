#include <boost/swap.hpp>
#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <iostream>

int main() {
    boost::tuple<int, std::string> t5 = {8, "Eight"};
    boost::tuple<int, std::string> t8 = {5, "Five"};

    std::cout << t5 << std::endl;
    std::cout << t8 << std::endl;

    std::cout << "Swapping..." << std::endl;
    boost::swap(t5, t8);

    std::cout << t5 << std::endl;
    std::cout << t8 << std::endl;

    return 0;
}
