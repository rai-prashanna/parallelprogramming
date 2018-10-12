#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void floydserial(int **current, int num, int **previous);
void floydparallel(int **current, int num, int **previous);


int main()
{
    int size = 2048, i = 0, j = 0, **previous = NULL,**current=NULL;
    const int MAX_RAND = 101; // If 101, we use infinity
    const int INFINITY = 9999;
    previous = (int**)malloc(size * sizeof(int*));
    current=(int**)malloc(size * sizeof(int*));
    for(i = 0; i < size; i++)
    {
        previous[i] = malloc(size * sizeof(int));
        current[i] = malloc(size * sizeof(int));
    }
    printf("Please enter the values for the distance previousrix of the directed graph. Enter 999 for infinity.\n");
    // Set the random values
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


    printf("The entered previousrix is : \n");
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
            printf("%d  ", previous[i][j]);
        printf("\n");
    }

    floydparallel(current, size,previous);
    printf("The resultant all-pairs shortest-paths previousrix is : \n");
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
            printf("%d  ", current[i][j]);
        printf("\n");
    }
    free(previous);
    free(current);

    return 0;
}

void floydparallel(int **current, int num, int **previous)
{
    int i = 0, j = 0, k = 0,start_index,end_index,col_chunk_size,id=0;
    int **swap;
    for(k = 0; k < num; k++)
        #pragma omp parallel firstprivate(num,k) default(none) private(i,j,swap,id,start_index,end_index) shared(current,previous,col_chunk_size) num_threads(32)
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

