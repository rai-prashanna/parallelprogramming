#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>
//#include <sys/resource.h>
#include <unistd.h>
#include <mpi.h>


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
    // Note that MPI executes all code in parallel that is not explicitly for a single rank
    unsigned long* primes;

    int rank, size, chunk_size, test_nr;

    // TODO: how should NPRIMES be passed in? If NPRIMES is small, then need extra code
        if(argc == 2)
        {
            int size = atoi(argv[1]);
            printf("testing get argument value: %d\n",size);

        }
    // I HOPE THIS IS WILL ANSWER YOUR TODO QUESTION
    
    NPRIMES = 100;
    unsigned long sqrt_num = (unsigned long) ceil(sqrt((unsigned long) NPRIMES));

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank==0) {
        // Serial section
        // The programs should take N = the number of max primes
        /*if(argc == 2)
        {
            NPRIMES = atoi(argv[1]);
        }
        else
        {
            printf("please supply arguments for number of max primes\n");
            exit(1);
        }*/

        printf("calculating prime numbers up to %lu \n", NPRIMES);

        primes = (unsigned long*)malloc(NPRIMES * sizeof(unsigned long));
        //unsigned long sqrt_num = (unsigned long) ceil(sqrt((unsigned long) NPRIMES));
        unsigned long leftnumbers= NPRIMES-sqrt_num;

        primes=parallelinit(primes,NPRIMES);

        timestamp_t t0 = get_timestamp();

        primes=seqfilterPrimesSqrt(2,primes,sqrt_num);
        printf("Done with seqfilterPrimesSqrt \n");

        // distribute CHUNKS to different processes
        chunk_size = leftnumbers / size;

        /*make iterations from 2 to NPRIMES and update counter with next prime number*/
        // For MPI, give each process a large section to work on and then send back result to process 0
        int i;

        // Each process not 0 should first wait to receive a chunk of numbers to work on
        // Process 0 sends this, then waits to get results back
        for (i=1; i<size; i++) {
            // TODO: send either the chunk size OR the starting limit to each process
            // Processes can use rank to calculate the start_index, end_index;
            printf("Process 0 now sending the chunk size to process %d \n", i);    
            MPI_Send(&chunk_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Collect the results from each process
        printf("Collecting the results from each process \n");

        for (i=1; i<size; i++) {
            //MPI_Recv(data, count, datatype, source, tag, communicator, status)
            MPI_Recv(&test_nr, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Process 0 received value %d from process %d \n", test_nr, i);    
        }

        displayPrimeNumbers(primes,NPRIMES);
        timestamp_t t1 = get_timestamp();
        double secs = (t1 - t0) / 1000000.0L;

        printf("execution time is   %lf \n",secs );

    }
    else {
        // Parallel section with the other processes
        int start_index, end_index;

        printf("Process %d now executing \n", rank);

        //MPI_Recv(data, count, datatype, source, tag, communicator, status)
        MPI_Recv(&chunk_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received chunk size %d from process 0\n", rank, chunk_size);

        int startingindex = sqrt_num+1;

        if(rank==1)
        {
            start_index=startingindex;
            end_index = chunk_size;
        }
        else if (rank == size - 1)
        {
            start_index = rank * chunk_size+1;
            end_index = NPRIMES;
        }
        else
        {
            start_index = rank * chunk_size+1;
            end_index = (rank+1)*chunk_size;
        }

        printf("Process %d would search primes between start index=%d to end index=%d \n", rank, start_index, end_index);

        // Send back a test result
        // TODO: probably need to send back an array of marken primes
        test_nr = rank + 100;
        MPI_Send(&test_nr, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        /*
        for(unsigned long i=start_index; i <= end_index; i =getNextPrime(i+1,primes))
        {

            for(unsigned long j = (i*2); j <= maxlimit; j += i)
            {
                *(primes +j) = 0;

            }
        }
        */
    }

    //primes=parallelfilterPrimes(sqrt_num+1,primes,NPRIMES,leftnumbers);

    MPI_Finalize();

    return 0;
}
