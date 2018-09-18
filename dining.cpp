#include <iostream>
#include <thread>
#include <mutex>

std::mutex out;

void philosopher(int n, std::mutex *left, std::mutex *right)
{
  while (true)
    {
      out.lock();
      std::cout << "Philosopher " << n << " is thinking." << std::endl;
      out.unlock();

      left->lock();
      out.lock();
      std::cout << "Philosopher " << n << " picked up her left fork." << std::endl;
      out.unlock();

      right->lock();
      out.lock();
      std::cout << "Philosopher " << n << " picked up her right fork." << std::endl;
      out.unlock();

      out.lock();
      std::cout << "Philosopher " << n << " is eating." << std::endl;
      out.unlock();

      out.lock();
      std::cout << "Philosopher " << n << " is putting down her right fork." << std::endl;
      out.unlock();
      right->unlock();

      out.lock();
      std::cout << "Philosopher " << n << " is putting down her left fork." << std::endl;
      out.unlock();
      left->unlock();
    }
}

void usage(char *program)
{
  std::cout << "Usage: " << program << " N  (where 2<=N<=10)" << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  if (argc != 2)
    {
      usage(argv[0]);
    }

  // philosophers = argv[1]
  int philosophers;
  try
    {
      philosophers = std::stoi(argv[1]);
    }
  catch (std::exception)
    {
      usage(argv[0]);
    }
  if (philosophers < 2 || philosophers > 10)
    {
      usage(argv[0]);
    }

  // forks
  std::mutex *forks = new std::mutex[philosophers];

  // philosophers
  std::thread *ph = new std::thread[philosophers];
  for (int i=0; i<philosophers; ++i)
    {
      int left = i;
      int right = (i == 0 ? philosophers : i) - 1;
      ph[i] = std::thread(philosopher, i, &forks[left], &forks[right]);
    }

  ph[0].join();
  delete[] forks;
  delete[] ph;

  return 0;
}
