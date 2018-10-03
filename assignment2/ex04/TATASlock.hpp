#ifndef TATASlock_hpp
#define TATASlock_hpp

#include <atomic>


class TATASlock
{

    // Make atomic std::atomic_bool
    std::atomic_bool value;

public:
    // default constructor
    //TATASlock() = default;
    TATASlock() {
        value = false;
    }

    ~TATASlock() {};

    void lock()
    {
        while(getAndSet(true));
    }

    void unlock()
    {
        getAndSet(false);
    }

private:

    bool getAndSet(bool newValue)
    {
        bool prior = value;
        value = newValue;
        return prior;
    }

} tlock;

#endif // tatas_lock
