#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <time.h>

typedef unsigned long long timestamp_t;

static timestamp_t
get_timestamp ()
{
    struct timeval now;
    gettimeofday (&now, NULL);
    return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

void floydserial(int **current, int num, int **previous);
void floydparallel(int **current, int num, int **previous,int numthreads);
void usage();


int main(int argc, char *argv[])
{
    int size = 2048, i = 0, j = 0, **previous = NULL,**current=NULL;
    const int MAX_RAND = 101; // If 101, we use infinity
    const int INFINITY = 9999;
    int numthreads=0;
    if (argc != 3)
    {
        usage();
    }

    if (argc == 3)
    {
        size = atoi(argv[1]);
        numthreads=atoi(argv[2]);
    }
    printf("Running standard version with size matrix = %d, number of threads=%d\n", size, numthreads);

    previous = (int**)malloc(size * sizeof(int*));
    current=(int**)malloc(size * sizeof(int*));
    for(i = 0; i < size; i++)
    {
        previous[i] = malloc(size * sizeof(int));
        current[i] = malloc(size * sizeof(int));
    }

    // Set the random values
    int seed = time(NULL);
    srand(seed);
    
    for(int r=0; r<size; r++)
    {
        for(int c=0; c<size; c++)
        {
            if (r==c)
            {
                previous[r][c] = 0;
                current[r][c] = 0;

            }
            else
            {
                // Sometimes, use inifinity as cost
                int w = rand() % MAX_RAND;

                if (w==MAX_RAND)
                {
                    w=INFINITY;
                }

                previous[r][c] = w;
                current[r][c] = w;
            }
        }
    }

    timestamp_t t0 = get_timestamp();

    floydparallel(current, size,previous,numthreads);

    timestamp_t t1 = get_timestamp();
    double secs = (t1 - t0) / 1000000.0L;

    printf("execution time is   %lf \n",secs );

    free(previous);
    free(current);

    return 0;
}

void floydparallel(int **current, int num, int **previous,int numthreads)
{
    int i = 0, j = 0, k = 0,start_index,end_index,col_chunk_size,id=0;
    int **swap;
    for(k = 0; k < num; k++)
        #pragma omp parallel firstprivate(num,k) default(none) private(i,j,swap,id,start_index,end_index) shared(current,previous,col_chunk_size) num_threads(numthreads)
    {

        id = omp_get_thread_num();

        int N_threads = omp_get_num_threads();
        /* distribute cols to different threads */
        col_chunk_size = num / N_threads;
        if(id==0)
        {
            start_index=0;
            end_index = col_chunk_size-1;
        }
        else
        {
            start_index = id * col_chunk_size;
            end_index = (id+1) * col_chunk_size-1;

        }

        if (id == N_threads - 1)
        {
            end_index = num-1;
        }
        for(i = start_index; i < end_index; i++)
        {
            for(j = start_index; j < end_index; j++)
            {
                {
                    if(i==j)
                    {
                        continue;
                    }
                    if(previous[i][k] + previous[k][j] < previous[i][j])
                    {
                        current[i][j] = previous[i][k] + previous[k][j];
                    }
                }
            }

        }
    }


    #pragma omp barrier
    {
        swap = current;
        current = previous;
        previous = swap;

    }


}

void floydserial(int **current, int num, int **previous)
{
    int i = 0, j = 0, k = 0;
    int **swap;
    for(k = 0; k < num; k++)
        for(i = 0; i < num; i++)
            for(j = 0; j < num; j++)
            {
                {
                    if(previous[i][k] + previous[k][j] < previous[i][j])
                        current[i][j] = previous[i][k] + previous[k][j];
                }
            }
    {
        swap = current;
        current = previous;
        previous = swap;

    }


}

void usage()
{
    printf("please supply argument for size of matrix and number of threads.\n");
    exit(1);
}
