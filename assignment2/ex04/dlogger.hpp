#ifndef dlogger_hpp
#define dlogger_hpp

#include <iostream>
#include <mutex>

bool doprint = true;
int min_level = 1;
int default_level = 0;

void dlog(const char* msg) {
    if (doprint && default_level >= min_level) {
        std::cout << "\n " << msg;
    }
}

void dlog(const char* msg, int level) {
    if (doprint && level >= min_level) {
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