/*
ex1.cpp
Group 55
2018-09-29
g++ -std=c++11 -pthread ex1.cpp -o ex1.exe
usage: ex1.exe T NT
*/

#include <iostream>
#include <string.h>
#include <thread>
#include <chrono>
#include <exception>
#include <stdexcept>
#include <mutex>

std::mutex mutex;

double shared_sum = 0.0;

void usage(char *program)
{
  std::cout << "Usage: " << program << " T NT" << std::endl;
  std::cout << std::endl;
  std::cout << "  T: number of threads" << std::endl;
  std::cout << "  NT: number of trapezes" << std::endl;
  exit(1);
}

void calcArea(int tn, int num_threads, int num_trapezes, double step) {
    // Calculates a rectangle area and adds the subtotal to shared_sum
    mutex.lock();
    std::cout << "Task " << tn << " is running" << std::endl;
    //std::cout << "Task " << tn << " is running with startidx " << startidx << std::endl;
    mutex.unlock();

  int i;
   double x, sum = 0.0;
  
    for(i=tn; i < num_trapezes; i+=num_threads) {
      /*x1 = i;
      x2 = i + step;
      sum = sum + ((x1 + x2)/2.0);*/

      //mutex.lock();
      //std::cout << "i " << i << ", num_trapezes " << num_trapezes << ", num_threads " << num_threads << std::endl;
      //mutex.unlock();

      //x = (startidx + i+0.5)*step;
      x = (i+0.5)*step;
      sum = sum + 4.0/(1.0+x*x);
    }

    mutex.lock();
    shared_sum = shared_sum + sum;
    mutex.unlock();
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

  // num_threads = argv[1]
  int num_threads;
  try
    {
      num_threads = std::stoi(argv[1]);
    }
  catch (std::exception)
    {
      usage(argv[0]);
    }
  if (num_threads < 1)
    {
      usage(argv[0]);
    }

  // num_trapezes = argv[2]
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

  // Limit the number of threads to num trapezes because using more threads does not make sense
  if (num_threads > num_trapezes) {
    std::cout << "Limiting the number of threads to number of trapezes." << std::endl;
    num_threads = num_trapezes;
  }

  double step, pi = 0.0;
  int i;

    // *** timing begins here ***
    auto start_time = std::chrono::system_clock::now();

    // Now calculate the areas
    // Area of each trapeze given by ( f(x2)+f(x1)/2 ) * step
    // To parallelize this, use a round robin for the threads
    
   // create and join threads
   std::thread *t = new std::thread[num_threads];

    step = 1.0/num_trapezes;
    //int num_trapezes_per_thread = num_trapezes/num_threads;

    for (int i=0; i<num_threads; ++i)
    {
      // call void calcArea(int tn, int startidx, int num_trapezes_per_thread, double step)
      //int startidx = i * num_trapezes_per_thread;
        t[i] = std::thread(calcArea, i, num_threads, num_trapezes, step);
    }

    for (int i=0; i<num_threads; ++i)
    {
        t[i].join();
    }

    pi = shared_sum * step;
    
    std::cout.precision(17);
    std::cout << std::fixed << pi << std::endl;

    std::chrono::duration<double> duration =
        (std::chrono::system_clock::now() - start_time);
    // *** timing ends here ***

    std::cout << "Finished in " << duration.count() << " seconds (wall clock)." << std::endl;

  return 0;
}
