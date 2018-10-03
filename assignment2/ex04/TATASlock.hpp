#ifndef TATASlock_hpp
#define TATASlock_hpp

#include <atomic>


class TATASlock
{
    // value must be atomic
    std::atomic<bool> value;

public:
    // default constructor
    //TATASlock() = default;
    TATASlock() {
        // No need for atomic operation in constructor
        value = false;
    }

    ~TATASlock() {};

    void lock()
    {
        while(
            //getAndSet(true)
            value.exchange(true)
        );
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

#endif // tatas_lock
