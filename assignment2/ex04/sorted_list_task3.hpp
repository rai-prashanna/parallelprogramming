#ifndef lacpp_sorted_list_hpp
#define lacpp_sorted_list_hpp lacpp_sorted_list_hpp

#include "TATASlock.hpp"
#include "dlogger.hpp"

/* a sorted list implementation by David Klaftenegger, 2015
 * please report bugs or suggest improvements to david.klaftenegger@it.uu.se
 */


/* struct for list nodes */
template<typename T>
struct node
{
    T value;
    node<T>* next;
};

/* thread-safe version of sorted singly-linked list */
/* Coarse-grained locking with TATAS lock */
template<typename T>
class sorted_list
{
    node<T>* first = nullptr;

	TATASlock tlock;

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

        tlock.lock();
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
		dlog("inside insert-unlock operation");
        tlock.unlock();
    }

    void remove(T v)
    {
        /* first find position */
        tlock.lock();
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
		   dlog("inside remove-unlock operation");
           tlock.unlock();
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
		dlog("inside remove-unlock operation");
        tlock.unlock();

    }

    /* count elements with value v in the list */
    std::size_t count(T v)
    {
        std::size_t cnt = 0;

		tlock.lock();
		dlog("inside count operation", 1);
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
		dlog("returning count operation", 1);
		tlock.unlock();
        return cnt;
    }
};

#endif // lacpp_sorted_list_hpp
