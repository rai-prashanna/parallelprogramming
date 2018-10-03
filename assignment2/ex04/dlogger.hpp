#ifndef dlogger_hpp
#define dlogger_hpp

#include <iostream>
#include <mutex>

bool doprint = false;

void dlog(const char* msg) {
    if (doprint) {
        std::cout << "\n " << msg;
    }
}

void dlog_with_lock(const char* msg) {
    if (doprint) {
        std::mutex mtx;

        mtx.lock();
        std::cout << "\n " << msg;
        mtx.unlock();
    }
}

#endif