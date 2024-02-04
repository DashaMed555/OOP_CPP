module;

#include <string>

export module module3;

export namespace Module3
{
    std::string getMyName() {
        std::string name = "Peter";
        return name;
    }
}
