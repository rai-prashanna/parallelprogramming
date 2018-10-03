/*
ex1.cpp
A sequential version of ex1.cpp
2018-09-26
g++ -std=c++11 ex1_seq.cpp -o ex1_seq.exe
usage: ex1_seq.exe T NT
*/

#include <iostream>
#include <string.h>
#include <chrono>
#include <exception>
#include <stdexcept>


void usage(char *program)
{
  std::cout << "Usage: " << program << " T NT" << std::endl;
  std::cout << std::endl;
  std::cout << "  T: number of threads" << std::endl;
  std::cout << "  NT: number of trapezes" << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        try
        {
            if(strcmp(argv[1],"-h")==0) {
                usage(argv[0]);
            }
            else
            {
                usage(argv[0]);
            }
        }
        catch(const std::exception& e) {
            std::cout << "Error: " << e.what() << "\"\n";
            usage(argv[0]);
        }
    }

    if (argc != 3)
    {
      usage(argv[0]);
    }

  // threads = argv[1]
  int threads;
  try
    {
      threads = std::stoi(argv[1]);
    }
  catch (std::exception)
    {
      usage(argv[0]);
    }
  if (threads < 1)
    {
      usage(argv[0]);
    }

  // trapezes = argv[2]
  int num_trapezes;
  try
    {
      num_trapezes = std::stoi(argv[2]);
    }
  catch (std::exception)
    {
      usage(argv[0]);
    }
  if (num_trapezes < 1)
    {
      usage(argv[0]);
    }

    std::cout << "Arguments entered correctly. Now running program." << std::endl;

  double step, x, sum = 0.0, pi = 0.0;
  int i;

    // *** timing begins here ***
    auto start_time = std::chrono::system_clock::now();

    // Now calculate the areas
    // Area of each trapeze given by ( f(x2)+f(x1)/2 ) * step
    step = 1.0/num_trapezes;
    for(i=0; i < num_trapezes; i++) {
      /*x1 = i;
      x2 = i + step;
      sum = sum + ((x1 + x2)/2.0);*/
      x = (i+0.5)*step;
      sum = sum + 4.0/(1.0+x*x);
    }
    pi = sum * step;
    
    std::cout.precision(17);
    std::cout << std::fixed << pi << std::endl;

    std::chrono::duration<double> duration =
        (std::chrono::system_clock::now() - start_time);
    // *** timing ends here ***

    std::cout << "Finished in " << duration.count() << " seconds (wall clock)." << std::endl;

  return 0;
}
