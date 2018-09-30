/*
Verify mutex support on the machine
g++ -std=c++11 -Wall -pthread verify_mutex.cpp -o verify_mutex
*/

#include <mutex>

int main()
{
    int i = 0;
    std::mutex m;
    m.lock();
    i = i + 1;
    m.unlock();
}
