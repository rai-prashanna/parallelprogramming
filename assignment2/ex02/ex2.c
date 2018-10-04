#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>
typedef unsigned long long timestamp_t;

static timestamp_t
    get_timestamp ()
    {
      struct timeval now;
      gettimeofday (&now, NULL);
      return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
    }



pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
long int NPRIMES=100000000;
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

long *init(long *primes,long NPRIMES)
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


void *filterPrimes(long startingindex,long *primes,long maxlimit)
{
    /*make iterations from 2 to NPRIMES and update counter with next prime number*/
    startingindex=getNextPrime(startingindex, primes);
    for(long i=startingindex; i <= maxlimit; i = getNextPrime(i+1, primes))
    {
        /*find multiple i.e j=i*2 and j=j+i and marked that number by setting array[index]=0 */
        for(long j = (i*2); j <= NPRIMES; j += i)
        {

            pthread_mutex_lock(&lock);
            *(primes +j) = 0;
            pthread_mutex_unlock(&lock);

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
    unsigned long startingindex =  targ->startindex;
    unsigned long upperlimit =  targ->upperlimit;

    filterPrimes(startingindex,targ->primes,upperlimit);

    return NULL;
}

int main(int argc, char* argv[])
{
    long* primes;
    int NUM_THREADS=4;
    pthread_t threads[NUM_THREADS]; /* An array of the threads. */
    struct thread_arg_t args[NUM_THREADS]; /* One argument struct per thread. */
    primes = (long*)malloc(NPRIMES * sizeof(long));
    unsigned long sqrt_num = (long) ceil(sqrt((double) NPRIMES));
    unsigned long leftnumbers= NPRIMES-sqrt_num;
    unsigned long equal_longervals=leftnumbers/NUM_THREADS;
    if( argc == 2 )
    {

         NUM_THREADS = atoi(argv[1]);
    }
    else
    {
        printf("please supply arugments for number of threads");
 	exit(1);
    }

    primes=init(primes,NPRIMES);

    filterPrimes(2,primes,sqrt_num);
    timestamp_t t0 = get_timestamp();
    filterPrimes(sqrt_num+1,primes,NPRIMES);
    /* Create (and run) all the threads. */
    for(long i=0,j=sqrt_num+1; i<NUM_THREADS; i++,j=j+equal_longervals+1)
    {
        args[i].startindex = j;
        args[i].upperlimit=j+equal_longervals;
        args[i].primes=primes;
        if(pthread_create(&threads[i], NULL,calculate_prime_pthread_runnable, &args[i]))
        {
            fprintf(stderr, "Error creating thread #%ld!\n", i);
            exit(1);
        }

    }

    /* Block until all threads are finished. */
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }


    primes=args[0].primes;
    timestamp_t t1 = get_timestamp();
double secs = (t1 - t0) / 1000000.0L;

printf("execution time is   %lf \n",secs );
    displayPrimeNumbers(primes,NPRIMES);

    return 0;

}

