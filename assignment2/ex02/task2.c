#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int NPRIMES=100;
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
    printf("inside getNextPrime");
    pthread_mutex_lock(&lock);
    while (*(x + i) == 0)
    {
        i++;
    }
    pthread_mutex_unlock(&lock);

    return i;
}

void displayPrimeNumbers(int *primes,int NPRIMES)
{
    printf("display all primes[] \n");

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

    }
    printf("init of primes[] \n");
    return primes;
}

//void *filterPrimes(int startingindex,int *primes,int maxlimit)
//{
//    printf("inside filterPrimes()");
//    /*make iterations from 2 to NPRIMES and update counter with next prime number*/
//    for(int i=startingindex; i <= maxlimit; i = getNextPrime(i+1, primes))
//    {
//        /*find multiple i.e j=i*2 and j=j+i and marked that number by setting array[index]=0 */
//        for(int j = (i*i); j < maxlimit; j += i)
//        {
////            printf("i: %d, j: %d\n", i, j);
//            if(j%i==0)
//            {
//                *(primes +j) = 0;
//                *(primes +i) = 0;
//
//            }
//
//        }
//
//    }
//    printf("exited filterPrimes()");
//    return NULL;
//}

void *filterPrimes(int startingindex,int *primes,int maxlimit)
{
    /*make iterations from 2 to NPRIMES and update counter with next prime number*/
    startingindex=getNextPrime(startingindex, primes);
    for(int i=startingindex; i <= maxlimit; i = getNextPrime(i+1, primes))
    {
        /*find multiple i.e j=i*2 and j=j+i and marked that number by setting array[index]=0 */
        for(int j = (i*2); j <= NPRIMES; j += i)
        {
            printf("i: %d, j: %d\n", i, j);
            *(primes +j) = 0;

        }

    }
    return NULL;
}

void *calculate_prime_pthread_runnable(void *arg)
{
    printf("inside runnable()");
    /* Cast the argument from void* to its proper type. */
    struct thread_arg_t *targ = arg;
    /* Add index to seed to make sure that different threads get different
     * seeds even if they are created so close in time that time() returns
     * the same value. */
    unsigned int startingindex =  targ->startindex;
    unsigned int upperlimit =  targ->upperlimit;

    filterPrimes(startingindex,targ->primes,upperlimit);

    printf("exited runnable()");
    return NULL;
}

int main(int argc, char* argv[])
{
    int* primes;
    int NUM_THREADS=1;
    NPRIMES = 100;
    pthread_t threads[NUM_THREADS]; /* An array of the threads. */
    struct thread_arg_t args[NUM_THREADS]; /* One argument struct per thread. */
    primes = (int*)malloc(NPRIMES * sizeof(int));
    unsigned int sqrt_num = (int) ceil(sqrt((double) NPRIMES));

    primes=init(primes,NPRIMES);
    filterPrimes(2,primes,sqrt_num);
    filterPrimes(sqrt_num+1,primes,NPRIMES);
//    /* Create (and run) all the threads. */
//args[0].primes=primes;
//    for (int i = sqrt_num+1,j=0; j < NUM_THREADS; j++)
//    {
//        if(i>=NPRIMES)
//        {
//            break;
//        }
//        args[j].startindex = i;
//        args[j].upperlimit=NPRIMES;
//
//        if (pthread_create(&threads[j], NULL,calculate_prime_pthread_runnable, &args[j]))
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
//
//    sleep();
//    primes=args[0].primes;
////displayPrimeNumbers(primes,NPRIMES);

    /* displayPrimeNumbers(primes,NPRIMES);
    */
    displayPrimeNumbers(primes,NPRIMES);

    return 0;

}
