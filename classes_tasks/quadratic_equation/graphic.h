#pragma once
#include <matplot/matplot.h>
#include <thread>


void plot_graph2(const double& a, const double& b, const double& c, const double& x1, const double& x2) {
    using namespace matplot;
    if (x1 < x2) {
        fplot([a, b, c](double x) { return a*x*x + b*x + c; }, std::array<double, 2>{x1 - abs(x1) - abs(x2), x2 + abs(x1) + abs(x2)}, "b");
    } else if (x1 > x2) {
        fplot([a, b, c](double x) { return a*x*x + b*x + c; }, std::array<double, 2>{x2 - abs(x1) - abs(x2), x1 + abs(x1) + abs(x2)}, "b");
    } else {
        fplot([a, b, c](double x) { return a*x*x + b*x + c; }, std::array<double, 2>{x1 - abs(a), x1 + abs(a)}, "b");
    } 
    
    grid(on);

    show();
}
void plot_graph1(const double& b, const double& c, const double& x1) {
    using namespace matplot;
    
    fplot([&b, &c](double x) { return b*x + c; }, std::array<double, 2>{x1 - abs(b)- abs(c), x1 + abs(b) + abs(c)}, "b");
    
    grid(on);

    show();
}