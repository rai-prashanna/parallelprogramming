#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

/* Define a data type for the arguments to pass to the functions of threads. */
struct thread_arg_t
{
    /* thread id */
    unsigned int id;

    /* starting index */
    unsigned int startindex;

    /* upper index */
    unsigned int upperlimit;

    /* calclauted primes */
    int* primes;


};

/* Returns the index of the next prime (non-zero, since we "knock out" non-primes by setting their value to zero) */
int getNextPrime(int i, int *x)
{
    while (*(x + i) == 0)
    {
        i++;
    }
    return i;
}

void displayPrimeNumbers(int *primes,int NPRIMES)
{
    for(int i=2; i < NPRIMES; i++)
    {
        if ( *(primes +i) != 0 )
        {
            printf("Prime number: %d\n", *(primes +i));
        }
    }
}

int *init(int *primes,int NPRIMES)
{
    /* we start at 2 because it is the smallest prime */
    /*list all numbers from 2 to max */
    for(int i=2; i < NPRIMES; i++)
    {
        *(primes +i) = i;
        printf("%d\n", i);
    }
    return primes;
}

void *filterPrimes(int startingindex,int *primes,int maxlimit)
{
    /*make iterations from 2 to NPRIMES and update counter with next prime number*/
    for(int i=startingindex; i <= maxlimit; i = getNextPrime(i+1, primes))
    {
        /*find multiple i.e j=i*2 and j=j+i and marked that number by setting array[index]=0 */
        for(int j = (i*2); j < maxlimit; j += i)
        {
            printf("i: %d, j: %d\n", i, j);
            primes[j] = 0;

        }

    }
    return NULL;
}

void *calculate_prime_pthread_runnable(void *arg)
{
    /* Cast the argument from void* to its proper type. */
    struct thread_arg_t *targ = arg;
    /* Add index to seed to make sure that different threads get different
     * seeds even if they are created so close in time that time() returns
     * the same value. */
    int*  resultprimes,tempprimes;
    unsigned int startingindex =  targ->startindex;
    unsigned int upperlimit =  targ->upperlimit;
    tempprimes= targ->primes;
    filterPrimes(startingindex,targ->primes,upperlimit);

    return NULL;
}

int main(int argc, char* argv[])
{
    int* primes;
    int NPRIMES;
    int NUM_THREADS=1;
    NPRIMES = 100;
    pthread_t threads[NUM_THREADS]; /* An array of the threads. */
    struct thread_arg_t args[NUM_THREADS]; /* One argument struct per thread. */
    primes = (int*)malloc(NPRIMES * sizeof(int));
    unsigned int sqrt_num = (int) ceil(sqrt((double) NPRIMES));

    primes=init(primes,NPRIMES);
    filterPrimes(2,primes,NPRIMES);
    displayPrimeNumbers(primes,NPRIMES);


    /* Create (and run) all the threads. */

//    primes=filterPrimes(2,primes,sqrt_num);
//
//    for (int i = sqrt_num+1,j=0; j < NUM_THREADS; j++)
//    {
//    if(i>=NPRIMES)
//    {
//    break;
//    }
//    args[j].startindex = i;
//    args[j].upperlimit=NPRIMES;
//    args[j].primes=primes;
//        if (pthread_create(&threads[i], NULL,calculate_prime_pthread_runnable, &args[j]))
//        {
//            fprintf(stderr, "Error creating thread #%d!\n", j);
//            exit(1);
//        }
//        i=i+1;
//
//    }
//
//    /* Block until all threads are finished. */
//    for (int i = 0; i < NUM_THREADS; i++)
//    {
//        pthread_join(threads[i], NULL);
//    }

//displayPrimeNumbers(primes,NPRIMES);

    /* displayPrimeNumbers(primes,NPRIMES);
    */

    return 0;

}
