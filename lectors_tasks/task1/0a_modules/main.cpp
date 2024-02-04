#include <iostream>

import module1;
import module2;
import module3;

int main(int argc, char** argv) {

    using namespace std;
    cout << "Hello world!" << "\n";

    cout << Module1::getMyName() << "\n";
    cout << Module2::getMyName() << "\n";

    using namespace Module1;
    cout << getMyName() << "\n"; // (A)
    cout << Module2::getMyName() << "\n";

    //using namespace Module2; // (B)
    //cout << getMyName() << "\n"; // COMPILATION ERROR (C)

    using Module2::getMyName;
    cout << getMyName() << "\n"; // (D)

    cout << Module3::getMyName() << "\n";

    return 0;
}
