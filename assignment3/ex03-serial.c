#include <stdio.h>
#include <stdlib.h>

void floyd(int**, int);

int main()
{
    int size = 4, i = 0, j = 0, **mat = NULL;

    mat = (int**)malloc(size * sizeof(int*));
    for(i = 0; i < size; i++)
        mat[i] = malloc(size * sizeof(int));
    printf("Please enter the values for the distance matrix of the directed graph. Enter 999 for infinity.\n");
    for(i = 0; i < size; i++)
        for(j = 0; j < size; j++)
            if( i == j )
            {
                mat[i][j] = 0;
            }
            else
                mat[i][j] = 1000;


        // initialize distances
    mat[0][1] = 3;
    mat[0][2] = 8;
    mat[1][3] = 1;
    mat[2][1] = 4;
    mat[3][0] = 2;
    mat[3][2] = -5;

    printf("The entered matrix is : \n");
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
            printf("%d  ", mat[i][j]);
        printf("\n");
    }

    floyd(mat, size);
    printf("The resultant all-pairs shortest-paths matrix is : \n");
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
            printf("%d  ", mat[i][j]);
        printf("\n");
    }
    free(mat);
    return 0;
}

void floyd(int **dist, int num)
{
    int i = 0, j = 0, k = 0;
    for(k = 0; k < num; k++)
        for(i = 0; i < num; i++)
            for(j = 0; j < num; j++)
            {
                if(dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
}

