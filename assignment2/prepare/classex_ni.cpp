// g++ -std=c++11 -Wall classex_ni.cpp -o classex_ni.exe
#include <iostream>

static long num_steps = 100;
double step;

int main()
{
    int i; double x, pi, sum = 0.0;

    step = 1.0/(double)num_steps;

    for(i = 0; i < num_steps; i++) {
        x = (i+0.5)*step;
        sum = sum + 4.0/(1.0+x*x);
    }
    pi = step * sum;
    std::cout.precision(17);
    std::cout << std::fixed << pi << std::endl;

    return(0);
}