#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <mpi.h>
#include <stdbool.h>

unsigned long NPRIMES=10000;

bool DEBUG_MODE = false;

typedef unsigned long long timestamp_t;
static timestamp_t
get_timestamp ()
{
    struct timeval now;
    gettimeofday (&now, NULL);
    return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}
/* Define a data type for the arguments to pass to the functions of threads. */



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
            printf("%ld   ", *(primes +i));
        }
    }
    printf("\n");
}

void displayNumbers(unsigned long *primes,unsigned long NPRIMES)
{
    printf("display all elements[] \n");

    for(unsigned long i=2; i < NPRIMES; i++)
    {
        printf("Number at index %d: %ld\n", i, *(primes +i));
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


unsigned long *seqfilterPrimesSqrt(unsigned long startingindex,unsigned long *primes,unsigned long maxlimit,unsigned long end_index)
{
    startingindex=getNextPrime(startingindex, primes);
    for(unsigned long i=startingindex; i <= end_index; i =getNextPrime(i+1,primes))
    {

        for(unsigned long j = (i*2); j <= maxlimit; j += i)
        {
            *(primes +j) = 0;

        }
    }

    return primes;
}

unsigned long *parallelfilterPrimes(unsigned long *primes,unsigned long start_index, unsigned long maxlimit,unsigned long end_index)
{
    start_index=getNextPrime(start_index, primes);
    for(unsigned long i=start_index; i <= end_index; i =getNextPrime(i+1,primes))
    {

        for(unsigned long j = (i*2); j <= maxlimit; j += i)
        {
            *(primes +j) = 0;

        }

    }
    return primes;
}

void copyValues(unsigned long *from, unsigned long *to, int size)
{
    // Copies the values of from to to
    for (int i=0; i<size; i++)
    {
        to[i] = from[i];
    }
}


int main(int argc, char* argv[])
{
    // Note that MPI executes all code in parallel that is not explicitly for a single rank

    int rank, size, chunksize, offset;
    timestamp_t t0, t1;

    unsigned long* primes;
    primes = (unsigned long*)malloc(NPRIMES * sizeof(unsigned long));

    /*
    // To finish, pass in NPRIMES. NPRIMES should be > 10
        if(argc == 2)
        {
            int size = atoi(argv[1]);
            printf("testing get argument value: %d\n",size);

        }
    */

    unsigned long sqrt_num = (unsigned long) ceil(sqrt((unsigned long) NPRIMES));
    unsigned long leftnumbers = NPRIMES-sqrt_num;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int otag = 1;
    int primestag = 2;

    // distribute CHUNKS to different processes
    chunksize = leftnumbers / size;
    if (rank==0)
    {
        /***** Master task only ******/
        // Serial section
        int i, aridx;

        unsigned long* partial_primes;
        partial_primes = (unsigned long*)malloc(NPRIMES * sizeof(unsigned long));

        printf("Calculating prime numbers up to %lu \n", NPRIMES);
        printf("Using %d number of processes \n", size);

        primes=parallelinit(primes,NPRIMES);

        t0 = get_timestamp();

        primes=seqfilterPrimesSqrt(2, primes, NPRIMES, sqrt_num);

        copyValues(primes, partial_primes, NPRIMES);
        MPI_Bcast(primes, NPRIMES, MPI_INT, 0, MPI_COMM_WORLD);
        /*make iterations from 2 to NPRIMES and update counter with next prime number*/
        // For MPI, give each process a large section to work on and then send back result to process 0

        // Collect the results and process the received partial primes
        printf("Collecting the results from each process \n");
        offset = sqrt_num;
        for (i=1; i<size; i++)
        {

            //    MPI_Recv(partial_primes, NPRIMES, MPI_INT, i, primestag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (DEBUG_MODE)
                printf("Process 0 received a completed primes from process %d \n", i);

            // set primes[aridx] = partial_primes[aridx] for this chunk
            for (aridx=offset; aridx<NPRIMES; aridx++)
            {
                if ( *(partial_primes + aridx) == 0 )
                {
                    *(primes + aridx) = 0;
                }
            }

            offset = offset + chunksize;
        }

        free(partial_primes);

    } /* end of master section */
    else
    {
        /***** Non-master tasks only *****/
        // Parallel section with the other processes
        int start_index, end_index;
        start_index = offset;

        if (rank == size - 1)
        {
            start_index = rank * chunksize+1;
            end_index = NPRIMES - 1;
        }
        else
        {
            start_index = rank * chunksize+1;
            end_index = (rank+1)*chunksize;
        }

        printf("Process %d now searching primes between start index=%d to end index=%d \n", rank, start_index, end_index);

        primes=parallelfilterPrimes(primes, start_index, NPRIMES, end_index);

        MPI_Reduce(primes,primes,NPRIMES,MPI_INT,MPI_SUM, 0, MPI_COMM_WORLD);

        // Send back the array of marked primes
        //     MPI_Send(primes, NPRIMES, MPI_INT, 0, primestag, MPI_COMM_WORLD);
    } /* end of non-master */

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
    {

        t1 = get_timestamp();
        double secs = (t1 - t0) / 1000000.0L;

        displayPrimeNumbers(primes,NPRIMES);

        printf("execution time is   %lf \n",secs );
        printf("Ran with %d number of ranks (processes) \n", size);

        printf("End of program \n");
    }

    free(primes);

    MPI_Finalize();

    return 0;
}

