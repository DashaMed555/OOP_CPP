module;

#include <string>

export module module1;

export namespace Module1
{
    std::string getMyName() {
        std::string name = "John";
        return name;
    }
}
