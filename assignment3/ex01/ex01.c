#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <omp.h>

typedef unsigned long long timestamp_t;
static timestamp_t
get_timestamp ()
{
    struct timeval now;
    gettimeofday (&now, NULL);
    return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}
unsigned long NPRIMES=100000000;
/* Define a data type for the arguments to pass to the functions of threads. */
int num_threads=2;


/* Returns the index of the next prime (non-zero, since we "knock out" non-primes by setting their value to zero) */
unsigned long getNextPrime(unsigned long i, unsigned long *x)
{
    while (*(x + i) == 0)
    {
        i=i+1;
    }
    return i;
}

void displayPrimeNumbers(unsigned long *primes,unsigned long NPRIMES)
{
    printf("display all primes[] \n");

    for(unsigned long i=2; i < NPRIMES; i++)
    {
        if ( *(primes +i) != 0 )
        {
            printf("Prime number: %ld\n", *(primes +i));
        }
    }
}

void displayNumbers(unsigned long *primes,unsigned long NPRIMES)
{
    printf("display all elements[] \n");

    for(unsigned long i=2; i < NPRIMES; i++)
    {
        printf("Prime number: %ld\n", *(primes +i));

    }
}

unsigned long *parallelinit(unsigned long *primes,unsigned long NPRIMES)
{
    /* we start at 2 because it is the smallest prime */
    /*list all numbers from 2 to max */
    for(unsigned long i=0; i < NPRIMES; i++)
    {
        *(primes +i) = i;

    }

    printf("init of primes[] \n");
    return primes;
}


unsigned long *parallelfilterPrimes(unsigned long startingindex,unsigned long *primes,unsigned long maxlimit,unsigned long leftnumbers)
{
    /*make iterations from 2 to NPRIMES and update counter with next prime number*/
    int  id, chunk_size, start_index, end_index,i,j;
    #pragma omp parallel firstprivate(maxlimit,leftnumbers,startingindex) private(i,j,start_index,end_index,id,chunk_size) shared(primes) num_threads(num_threads) default(none)
    {
        id = omp_get_thread_num();
        int N_threads = omp_get_num_threads();
        /* distribute CHUNKS to different threads */
        chunk_size = leftnumbers / N_threads;
        if(id==0)
        {
            start_index=startingindex;
            end_index = chunk_size;

        }
        else if (id == N_threads - 1)
        {
            start_index = id * chunk_size+1;

            end_index = maxlimit;
        }
        else
        {
            start_index = id * chunk_size+1;
            end_index = (id+1)*chunk_size;

        }


        omp_set_dynamic(0);
        omp_set_num_threads(N_threads);

        for(unsigned long i=start_index; i <= end_index; i =getNextPrime(i+1,primes))
        {

            for(unsigned long j = (i*2); j <= maxlimit; j += i)
            {
                *(primes +j) = 0;

            }

        }
    }

    return primes;
}




unsigned long *seqfilterPrimesSqrt(unsigned long startingindex,unsigned long *primes,unsigned long maxlimit)
{
        for(unsigned long i=startingindex; i <= maxlimit; i =getNextPrime(i+1,primes))
        {

            for(unsigned long j = (i*2); j <= maxlimit; j += i)
            {
                *(primes +j) = 0;

            }

        }


    return primes;
}


int main(int argc, char* argv[])
{
    unsigned long* primes;
    int NUM_THREADS=4;

    if( argc == 3 )
    {
        NUM_THREADS = atoi(argv[1]);
        NPRIMES=atoi(argv[2]);
        printf("number of threads %d",NUM_THREADS);
        printf("calculating prime numbers upto %lu",NUM_THREADS);
    }
    else
    {
        printf("please supply arugments for number of threads\n");
        exit(1);
    }

//    NUM_THREADS=4;

    printf("Using %d threads with max = %ld \n", NUM_THREADS, NPRIMES);

    primes = (unsigned long*)malloc(NPRIMES * sizeof(unsigned long));
    unsigned long sqrt_num = (unsigned long) ceil(sqrt((unsigned long) NPRIMES));
    unsigned long leftnumbers= NPRIMES-sqrt_num;

    primes=parallelinit(primes,NPRIMES);

    timestamp_t t0 = get_timestamp();

    primes=seqfilterPrimesSqrt(2,primes,sqrt_num);

    primes=parallelfilterPrimes(sqrt_num+1,primes,NPRIMES,leftnumbers);
  displayPrimeNumbers(primes,NPRIMES);
    timestamp_t t1 = get_timestamp();
    double secs = (t1 - t0) / 1000000.0L;


    printf("execution time is   %lf \n",secs );

    return 0;
}
