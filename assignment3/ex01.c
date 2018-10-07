#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <omp.h>


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
long int NPRIMES=10000000;
/* Define a data type for the arguments to pass to the functions of threads. */
struct thread_arg_t
{
    /* thread id */
    unsigned long id;

    /* starting index */
    unsigned long startindex;

    /* upper index */
    unsigned long upperlimit;

    /* calclauted primes */
    long* primes;


};

/* Returns the index of the next prime (non-zero, since we "knock out" non-primes by setting their value to zero) */
long getNextPrime(long i, long *x)
{
    pthread_mutex_lock(&lock);
    while (*(x + i) == 0)
    {
        i=i+1;
    }
    pthread_mutex_unlock(&lock);

    return i;
}

void displayPrimeNumbers(long *primes,long NPRIMES)
{
    printf("display all primes[] \n");

    for(long i=2; i < NPRIMES; i++)
    {
        if ( *(primes +i) != 0 )
        {
            printf("Prime number: %ld\n", *(primes +i));
        }
    }
}

long *parallelinit(long *primes,long NPRIMES)
{
    /* we start at 2 because it is the smallest prime */
    /*list all numbers from 2 to max */
    #pragma omp parallel for schedule (dynamic)
    for(long i=2; i < NPRIMES; i++)
    {
        *(primes +i) = i;

    }
    printf("init of primes[] \n");
    return primes;
}

long *serialinit(long *primes,long NPRIMES)
{
    /* we start at 2 because it is the smallest prime */
    /*list all numbers from 2 to max */
    for(long i=2; i < NPRIMES; i++)
    {
        *(primes +i) = i;

    }
    printf("init of primes[] \n");
    return primes;
}


void *parallelfilterPrimes(long startingindex,long *primes,long maxlimit)
{
    /*make iterations from 2 to NPRIMES and update counter with next prime number*/
    int nextindex=startingindex;
    startingindex=getNextPrime(startingindex, primes);
    for(long i=startingindex; i <= maxlimit; i = nextindex)
    {
        #pragma omp parallel for default(none) firstprivate(i,NPRIMES) shared(primes) schedule (dynamic)
        for(long j = (i*2); j <= NPRIMES; j += i)
        {

            *(primes +j) = 0;

        }
        nextindex=getNextPrime(i+1,primes);


    }

}

void *serialfilterPrimes(long startingindex,long *primes,long maxlimit)
{
    /*make iterations from 2 to NPRIMES and update counter with next prime number*/
    int nextindex=startingindex;
    startingindex=getNextPrime(startingindex, primes);
    for(long i=startingindex; i <= maxlimit; i = nextindex)
    {
        #pragma omp parallel for default(none) firstprivate(i,NPRIMES) shared(primes) schedule (dynamic)
        for(long j = (i*2); j <= NPRIMES; j += i)
        {

            *(primes +j) = 0;

        }
        nextindex=getNextPrime(i+1,primes);


    }

}


int main(int argc, char* argv[])
{
    long* primes;
    int NUM_THREADS=1;
    pthread_t threads[NUM_THREADS]; /* An array of the threads. */
    struct thread_arg_t args[NUM_THREADS]; /* One argument struct per thread. */
    primes = (long*)malloc(NPRIMES * sizeof(long));
    unsigned long sqrt_num = (long) ceil(sqrt((double) NPRIMES));
    unsigned long leftnumbers= NPRIMES-sqrt_num;
    unsigned long equal_longervals=leftnumbers/NUM_THREADS;

    primes=parallelinit(primes,NPRIMES);

    parallelfilterPrimes(2,primes,NPRIMES);

    displayPrimeNumbers(primes,NPRIMES);

    return 0;

}


