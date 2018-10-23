#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define  ARRAYSIZE	1600000
#define  MASTER		0
unsigned long* primes;


/* Returns the index of the next prime (non-zero, since we "knock out" non-primes by setting their value to zero) */
unsigned long getNextPrime(unsigned long i, unsigned long *x)
{
    while (*(x + i) == 0)
    {
        i=i+1;
    }
    return i;
}

unsigned long *init(unsigned long *primes,unsigned long NPRIMES)
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

unsigned long *parallelfilterPrimes(unsigned long *primes,unsigned long start_index, unsigned long maxlimit,unsigned long end_index)
{
    for(unsigned long i=start_index; i <= end_index; i =getNextPrime(i+1,primes))
    {

        for(unsigned long j = (i*2); j <= maxlimit; j += i)
        {
            *(primes +j) = 0;

        }

    }
    return primes;
}



unsigned long *update(unsigned long *primes,int myoffset, int chunk, int myid, int numtasks)
{
    int start_index, end_index,i,j;
    float mysum;
    /* Perform addition to each of my array elements and keep my sum */
    mysum = 0;
    /*make iterations from 2 to NPRIMES and update counter with next prime number*/
    if(myid==0)
    {
        start_index=0;
        end_index = chunk;

    }
    else if (myid == numtasks - 1)
    {
        start_index = myid * chunk+1;

        end_index = ARRAYSIZE;
    }
    else
    {
        start_index = myid * chunk+1;
        end_index = (myid+1)*chunk;

    }

    primes=parallelfilterPrimes(primes,start_index,ARRAYSIZE,end_index);


    // printf("Task %d mysum = %e\n",myid,mysum);
    // printf("Task %d arraysize = %zu\n",myid,actualsize);

    return primes;
}




int main (int argc, char *argv[])
{
    int   numtasks, taskid, rc, dest, offset, i, j, tag1,
          tag2, source, chunksize;
    float mysum, sum;
    primes = (unsigned long*)malloc(ARRAYSIZE * sizeof(unsigned long));
    //float update(int myoffset, int chunk, int myid);
    MPI_Status status;

    /***** Initializations *****/
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    if (numtasks % 4 != 0)
    {
        printf("Quitting. Number of MPI tasks must be divisible by 4.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
        exit(0);
    }
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    //  printf ("MPI task %d has started...\n", taskid);
    chunksize = (ARRAYSIZE / numtasks);
    tag2 = 1;
    tag1 = 2;

    /***** Master task only ******/
    if (taskid == MASTER)
    {

        /* Initialize the array */
//        sum = 0;
//        for(i=0; i<ARRAYSIZE; i++)
//        {
//            data[i] =  i * 1.0;
//            sum = sum + data[i];
//        }

        primes=init(primes,ARRAYSIZE);

        printf("Initialized array sum = %e\n",sum);

        /* Send each task its portion of the array - master keeps 1st part */
        offset = chunksize;
        for (dest=1; dest<numtasks; dest++)
        {
            MPI_Send(&offset, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD);
            MPI_Send(primes, ARRAYSIZE, MPI_FLOAT, dest, tag2, MPI_COMM_WORLD);
            //  printf("Sent %d elements to task %d offset= %d\n",chunksize,dest,offset);
            offset = offset + chunksize;
        }

        /* Master does its part of the work */
        offset = 0;
//        mysum = update(offset, chunksize, taskid);

        /* Wait to receive results from each task */
        for (i=1; i<numtasks; i++)
        {
            source = i;
            MPI_Recv(&offset, 1, MPI_INT, source, tag1, MPI_COMM_WORLD, &status);
            MPI_Recv(primes, ARRAYSIZE, MPI_FLOAT, source, tag2,
                     MPI_COMM_WORLD, &status);
            {
                //printf("Task with task id  %d array element = %f\n",source,(primes +i));

            }

        }

        printf("Got it \n");

        /* Get final sum and print sample results */
//        MPI_Reduce(&mysum, &sum, 1, MPI_FLOAT, MPI_SUM, MASTER, MPI_COMM_WORLD);
////        printf("Sample results: \n");
//        offset = 0;
//        for (i=0; i<numtasks; i++)
//        {
//            for (j=0; j<5; j++)
//            {
//                //    printf("  %e",data[offset+j]);
//                // printf("\n");
//                offset = offset + chunksize;
//
//            }
//        }
//        printf("*** Final sum= %e ***\n",sum);

        printf("Sample results: \n");
        offset = 0;
        for (int i=0; i<numtasks; i++)
        {
            for (int j=0; j<5; j++)
                printf("\n");
            offset = offset + chunksize;
        }


        /* Get final sum and print sample results */
        printf("Sample results: \n");
        offset = 0;
        for (i=0; i<numtasks; i++)
        {
            for (j=0; j<5; j++)
                printf("Prime number: %ld\n", *(primes +(offset+j)));
            printf("\n");
            offset = offset + chunksize;
        }
// printf("*** Final sum= %e ***\n",sum);

    }  /* end of master section */


    /***** Non-master tasks only *****/

    if (taskid > MASTER)
    {

        /* Receive my portion of array from the master task */
        source = MASTER;
        MPI_Recv(&offset, 1, MPI_INT, source, tag1, MPI_COMM_WORLD, &status);
        MPI_Recv(primes, ARRAYSIZE, MPI_FLOAT, source, tag2,
                 MPI_COMM_WORLD, &status);
        primes = update(primes,offset, chunksize, taskid,numtasks);

        /* Send my results back to the master task */
        dest = MASTER;
        MPI_Send(&offset, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD);
        MPI_Send(primes, ARRAYSIZE, MPI_FLOAT, MASTER, tag2, MPI_COMM_WORLD);



        //MPI_Reduce(&mysum, &sum, 1, MPI_FLOAT, MPI_SUM, MASTER, MPI_COMM_WORLD);

    } /* end of non-master */


    MPI_Finalize();

}   /* end of main */





