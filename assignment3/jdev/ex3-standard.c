// gcc ex3-standard.c -o ex3-standard
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


const int MAX_RAND = 101; // If 101, we use infinity
const int INFINITY = 9999;

void initMatrix(int **mtx, int size);
void prettyPrintMatrix(int **mtx, int size);
void floydWarshallStandard(int **mtx, int size);
void usage();

int main (int argc, char *argv[])
{
    int n_mtxsize;

    if (argc != 2)
    {
      usage();
    }

    if (argc == 2)
    {
        if(argv[1] == "-h") {
            usage();
        }
        else {
           n_mtxsize = atoi(argv[1]);
        }
    }

    printf("Running with size matrix = %d\n\n", n_mtxsize);

    int **D_matrix;

    initMatrix(D_matrix, n_mtxsize);

    prettyPrintMatrix(D_matrix, n_mtxsize);

    floydWarshallStandard(D_matrix, n_mtxsize);

    prettyPrintMatrix(D_matrix, n_mtxsize);
    
    free(D_matrix);

    return 0;
}

void floydWarshallStandard(int **mtx, int size) {
    // In k=0, we set the edge costs
    for (int k=1; k<size; k++) {
        printf("Now on k = %d\n", k);
        for(int j=1; j<size; ++j) {
            for(int i=1; i<size; ++i) { 
                // Set mtx[i][j] for k+1 to the minimum
                // can skip where i=j
                if (i==j)  continue;

                int tmpcost = mtx[i][k] + mtx[k][j];
                if (tmpcost < mtx[i][j]) {
                    printf("Changing cost from %d to %d\n", mtx[i][k], tmpcost);
                    mtx[i][j] = tmpcost;
                }
                else {
                    printf("Keeping current cost at %d\n", mtx[i][j]);
                }
            }
        }
        prettyPrintMatrix(mtx, size);
    }
}

void initMatrix(int **mtx, int size) {
  int seed = time(NULL);
  srand(seed);

  for(int r=0; r<size; r++)
  {
    mtx[r] = (int*)malloc(size*sizeof(int));
  }

  // Set the random values
  for(int r=0; r<size; r++)
  {
    for(int c=0; c<size; c++)
    {
        if (r==c) {
            mtx[r][c] = 0;
        }
        else {
            // Sometimes, use inifinity as cost
            int w = rand() % MAX_RAND;

            if (w==MAX_RAND) {
                w=INFINITY;
            }

            mtx[r][c] = w;
        }
    }
  }
}
void prettyPrintMatrix(int **mtx, int size) {
    for(int i=0; i<size; ++i) {
        printf("\n\n");
        for(int j=0; j<size; ++j)
        {
            printf("%d  ", mtx[i][j]);
        }
    }
    printf("\n");
}

void usage()
{
  printf("please supply arugment for size of matrix");
  exit(1);
}
