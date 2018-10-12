#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <omp.h>

typedef unsigned long long timestamp_t;

static timestamp_t
get_timestamp ()
{
    struct timeval now;
    gettimeofday (&now, NULL);
    return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

void floydserial(int **current, int num, int **previous);
void floydparallel(int **current, int num, int **previous);
void usage();


int main (int argc, char *argv[])
{
    const int MAX_RAND = 101; // If 101, we use infinity
    const int INFINITY_VAL = 9999;

    int size, i = 0, j = 0, **previous = NULL,**current=NULL;

    if (argc != 2)
    {
        usage();
    }

    if (argc == 2)
    {
        size = atoi(argv[1]);
    }
    printf("Running with size matrix = %d\n", size);

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
                    w=INFINITY_VAL;
                }

                previous[r][c] = w;
                current[r][c] = w;
            }
        }
    }

    printf("The original matrix is : \n");
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
            printf("%d  ", previous[i][j]);
        printf("\n");
    }

    timestamp_t t0 = get_timestamp();

    floydparallel(current, size,previous);

    timestamp_t t1 = get_timestamp();

    printf("The resultant all-pairs shortest-paths matrix is : \n");
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
            printf("%d  ", current[i][j]);
        printf("\n");
    }

    double secs = (t1 - t0) / 1000000.0L;

    printf("execution time is   %lf \n",secs );

    free(previous);
    free(current);

    return 0;
}

void floydparallel(int **current, int num, int **previous)
{
    int i = 0, j = 0, k = 0;
    int **swap;
    for(k = 0; k < num; k++)
        #pragma omp parallel for firstprivate(num) private(swap) collapse(2) shared(current,previous) num_threads(1)
        for(i = 0; i < num; i++)
            for(j = 0; j < num; j++)
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
    printf("please supply argument for size of matrix\n");
    exit(1);
}
