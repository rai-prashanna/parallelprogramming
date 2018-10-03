#ifndef TATASlock_hpp
#define TATASlock_hpp

#include <atomic>


class TATASlock
{
    // value must be atomic
    std::atomic<bool> value;

public:
    // constructor
    TATASlock() {
        // No need for atomic operation in constructor
        value = false;
    }

    ~TATASlock() {};

    void lock()
    {
        //getAndSet(true)
        while (true) {
            // Wait until lock looks free
            while (value.load()) {}
            // Then try try to acquire the lock
            if (!value.exchange(true)) {
                return;
            }
        };
    }

    void unlock()
    {
        value.store(false);
    }

private:

    bool getAndSet(bool newValue)
    {
        bool prior = value.load();
        value.store(newValue);
        return prior;
    }

} tlock;

#endif
