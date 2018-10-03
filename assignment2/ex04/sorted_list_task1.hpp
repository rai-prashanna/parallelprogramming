#ifndef lacpp_sorted_list_hpp
#define lacpp_sorted_list_hpp lacpp_sorted_list_hpp
#include <mutex>
#include "dlogger.hpp"

/* a sorted list implementation by David Klaftenegger, 2015
 * please report bugs or suggest improvements to david.klaftenegger@it.uu.se
 */

std::mutex mtx;

/* struct for list nodes */
template<typename T>
struct node
{
    T value;
    node<T>* next;
};

/* thread-safe version of sorted singly-linked list */
/* Coarse-grained locking with mutex */
template<typename T>
class sorted_list
{
    node<T>* first = nullptr;

public:
    /* default implementations:
     * default constructor
     * copy constructor (note: shallow copy)
     * move constructor
     * copy assignment operator (note: shallow copy)
     * move assignment operator
     *
     * The first is required due to the others,
     * which are explicitly listed due to the rule of five.
     */
    sorted_list() = default;
    sorted_list(const sorted_list<T>& other) = default;
    sorted_list(sorted_list<T>&& other) = default;
    sorted_list<T>& operator=(const sorted_list<T>& other) = default;
    sorted_list<T>& operator=(sorted_list<T>&& other) = default;
    ~sorted_list()
    {
        while(first != nullptr)
        {
            remove(first->value);
        }
    }
    /* insert v into the list */
    void insert(T v)
    {
        /* first find position */

        mtx.lock();
        dlog("inside insert-lock operation");

        node<T>* pred = nullptr;
        node<T>* succ = first;
        while(succ != nullptr && succ->value < v)
        {
            pred = succ;
            succ = succ->next;
        }
        dlog("inside insert-unlock operation");

        /* construct new node */
        node<T>* current = new node<T>();
        current->value = v;

        /* insert new node between pred and succ */


        current->next = succ;
        if(pred == nullptr)
        {
            first = current;
        }
        else
        {
            pred->next = current;
        }
        mtx.unlock();
        dlog("inside insert-unlock operation");

    }

    void remove(T v)
    {
        /* first find position */
        mtx.lock();
        dlog("inside remove-lock operation");

        node<T>* pred = nullptr;
        node<T>* current = first;
        while(current != nullptr && current->value < v)
        {
            pred = current;
            current = current->next;
        }
        if(current == nullptr || current->value != v)
        {
          mtx.unlock();
        	dlog("inside remove-unlock operation");
            /* v not found */
            return;
        }
        /* remove current */
        if(pred == nullptr)
        {
            first = current->next;
        }
        else
        {
            pred->next = current->next;
        }
        delete current;
        mtx.unlock();
        dlog("inside remove-unlock operation");

    }

    /* count elements with value v in the list */
    // count does not need locking
    std::size_t count(T v)
    {
        std::size_t cnt = 0;

        mtx.lock();
        /* first go to value v */
        node<T>* current = first;
        while(current != nullptr && current->value < v)
        {
            current = current->next;
        }
        /* count elements */
        while(current != nullptr && current->value == v)
        {
            cnt++;
            current = current->next;
        }
        mtx.unlock();
        return cnt;
    }
};

#endif // lacpp_sorted_list_hpp
