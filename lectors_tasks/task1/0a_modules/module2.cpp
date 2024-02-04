module;

#include <string>

export module module2;

export namespace Module2
{
    std::string getMyName() {
        std::string name = "James";
        return name;
    }
}
