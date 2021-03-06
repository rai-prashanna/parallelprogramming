/*
template.cpp
This is code that can be used in the other programs to get started.
A sequential version.
2018-09-28
g++ template.cpp -o template.exe
g++ -std=c++11 -Wall -pthread template.cpp -o template.exe
usage: template.exe T NT
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
  int trapezes;
  try
    {
      trapezes = std::stoi(argv[2]);
    }
  catch (std::exception)
    {
      usage(argv[0]);
    }
  if (trapezes < 1)
    {
      usage(argv[0]);
    }

    std::cout << "Arguments entered correctly. Now running program." << std::endl;

    // *** timing begins here ***
    auto start_time = std::chrono::system_clock::now();

    std::chrono::duration<double> duration =
        (std::chrono::system_clock::now() - start_time);
    // *** timing ends here ***

    std::cout << "Finished in " << duration.count() << " seconds (wall clock)." << std::endl;

  return 0;
}
