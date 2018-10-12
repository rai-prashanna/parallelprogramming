#include <stdio.h>
#include <stdlib.h>

void floydserial(int **current, int num, int **previous);
void floydparallel(int **current, int num, int **previous);


int main()
{
    int size = 16, i = 0, j = 0, **previous = NULL,**current=NULL;
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
    int i = 0, j = 0, k = 0;
    int **swap;
    for(k = 0; k < num; k++)
        #pragma omp parallel for firstprivate(num) private(swap) collapse(2) shared(current,previous)
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

