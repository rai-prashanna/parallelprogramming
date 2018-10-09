#include <stdio.h>
#include <stdlib.h>

void floyd(int**, int,int**);

int main()
{
    int size = 4, i = 0, j = 0, **previous = NULL,**current=NULL;

    previous = (int**)malloc(size * sizeof(int*));
    current=(int**)malloc(size * sizeof(int*));
    for(i = 0; i < size; i++)
    {
        previous[i] = malloc(size * sizeof(int));
        current[i] = malloc(size * sizeof(int));
    }
    printf("Please enter the values for the distance previousrix of the directed graph. Enter 999 for infinity.\n");
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
        {
            if( i == j )
            {
                previous[i][j] = 0;
                current[i][j] = 0;

            }
            else
            {

                previous[i][j] = 1000;
                current[i][j] = 1000;
            }

            // initialize distances
            previous[0][1] = rand() % 6;
            current[0][1] = previous[0][1] ;

            previous[0][2] = rand() % 8;
            current[0][2] = previous[0][2];

            previous[1][3] = rand() % 5;
            current[1][3] = previous[1][3];

            previous[2][1] = rand() % 9;
            current[2][1] = previous[2][1];

            previous[3][0] = rand() % 3;
            current[3][0] =    previous[3][0];

            previous[3][2] = rand() % 5 -1;
            current[3][2] =    previous[3][2];
        }
    }
    printf("The entered previousrix is : \n");
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
            printf("%d  ", previous[i][j]);
        printf("\n");
    }

    floyd(current, size,previous);
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

void floyd(int **current, int num, int **previous)
{
    int i = 0, j = 0, k = 0;
    int **swap;
    #pragma omp parallel for firstprivate(num) private(swap) collapse(3) shared(dist,num,current,previous)
    for(k = 0; k < num; k++)
        for(i = 0; i < num; i++)
            for(j = 0; j < num; j++)
            {
                if(previous[i][k] + previous[k][j] < previous[i][j])
                    #pragma omp atomic
                    current[i][j] = previous[i][k] + previous[k][j];
            }


    #pragma omp barrier
    {
        swap = current;
        current = previous;
        previous = swap;

    }


}

