#include <iostream>
#include <cmath>
#include "graphic.h"

using namespace std;

int main() {
    double a, b, c, d, x1, x2;

    cout << "Enter a\n";
    cin >> a;
    cout << "Enter b\n";
    cin >> b;
    cout << "Enter c\n";
    cin >> c;

    d = b * b - 4 * a * c;
    if (d > 0) {
        x1 = ((-b) + sqrt(d)) / (2 * a);
        x2 = ((-b) - sqrt(d)) / (2 * a);
        plot_graph2(a, b, c, x1, x2);
    }
    if (d == 0) {
        x1 = -(b / (2 * a));
        plot_graph1(b, c, x1);
    }

}
