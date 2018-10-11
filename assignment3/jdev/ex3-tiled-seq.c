// A tiled version of FW
// gcc -Wall ex3tiled.c -o ex3tiled
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct {
    int id;
    int offset_i;
    int offset_j;
} tile;


#define CACHE_LINE_SIZE = 64;
const int intsizebytes = 4;
// Set tile size to a 4x4 matrix (64/4) = 16 integer values. So 4x4
const int B_TILE_SIZE = 4;
//const int B_TILE_SIZE = 16;

const int MAX_RAND = 101; // If 101, we use infinity
const int INFINITY = 9999;
int * pInt = NULL;

int ** initMatrix(int **mtx, int size);
void prettyPrintMatrix(int **mtx, int size);
void floydWarshallTiled(int **mtx, int size);
void floydWarshallTileCore(int **mtx, int size, int k);
//void floydWarshallCore(int **mtx, int size, tile t);
void floydWarshallStandard(int **mtx, int size);
int ** initNextMatrix(int **mtx, int size, int **frommtx);
void copyMatrixValues(int **frommtx, int **tomtx, int size);
//int ** copyDMatrix(int **frommtx, int **tomtx, int size);
//bool isNullTile(tile t);
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
        n_mtxsize = atoi(argv[1]);
        if (n_mtxsize < B_TILE_SIZE) {
            usage();
        }
    }

    printf("Running with size matrix = %d\n", n_mtxsize);
    printf("Tiled version with B tile size = %d\n\n", B_TILE_SIZE);

    int ** D_matrix = malloc(n_mtxsize*sizeof(int*));
    //mtx = aligned_alloc(16, n_mtxsize*sizeof(int*));

    int ** D_matrix_nextk = malloc(n_mtxsize*sizeof(int*));
    //mtx = aligned_alloc(16, n_mtxsize*sizeof(int*));

    D_matrix = initMatrix(D_matrix, n_mtxsize);
    printf("Init the matrix done\n");

    prettyPrintMatrix(D_matrix, n_mtxsize);

    printf("Init the next k+1 matrix\n");
    D_matrix_nextk = initNextMatrix(D_matrix_nextk, n_mtxsize, D_matrix);
    printf("Init the next k+1 matrix done. Here it is:\n");
    prettyPrintMatrix(D_matrix_nextk, n_mtxsize);

    floydWarshallTiled(D_matrix, n_mtxsize);
    printf("floydWarshallTiled done\n");

    prettyPrintMatrix(D_matrix, n_mtxsize);

    printf("Freeing resources\n");
    free(D_matrix);
    free(D_matrix_nextk);
    printf("Done\n");

    return 0;
}

void floydWarshallTiled(int **mtx, int size) {
    // In k=0, we set the edge costs
    // n step B: skip every B-tile size
    for (int k=0; k<size; k=k+B_TILE_SIZE) {
        printf("Now on k = %d\n", k);
        // CR tile
        /*tile cr_tile = {
            .id = 1,
            .offset_i = 0,  // B_TILE_SIZE
            .offset_j = 0   // B_TILE_SIZE
        };*/
        
        floydWarshallTileCore(mtx, size, k);
        //floydWarshallCore(mtx, B_TILE_SIZE, cr_tile);

        // TODO: E, W, N, S tiles
        // Is it better to collect tiles in array or like below? For parallelize?
        int t_base_col_j = floor(k / size)*size;
        int t_base_row_i = floor(k % B_TILE_SIZE)-1;
        for (int t=t_base_col_j; t_base_col_j<t_base_col_j+B_TILE_SIZE; t=t+B_TILE_SIZE) {
            // These iterates over E, W tiles
            // Skip cr tile
            if (t==k) continue;
            printf("Would iterate over tiles with k %d\n", t);
        }

        // TODO: NE, NW, SE, SW tiles

    }
}

void floydWarshallTileCore(int **mtx, int size, int k) {
    printf("  floydWarshallCore now k = %d\n", k);
    for(int j=0; j<size; ++j) {
        for(int i=0; i<size; ++i) {
            // Set mtx[i][j] for k+1 to the minimum
            // can skip where i=j
            if (i==j)  continue;

            int tmpcost = mtx[i][k] + mtx[k][j];
            if (tmpcost < mtx[i][j]) {
                printf("Changing cost from %d to %d\n", mtx[i][j], tmpcost);
                mtx[i][j] = tmpcost;
            }
            else {
                printf("Keeping current cost at %d\n", mtx[i][j]);
            }
        }
    }
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
        //prettyPrintMatrix(mtx, size);
    }
}

int ** initMatrix(int **mtx, int size) {
  int seed = time(NULL);
  srand(seed);

  for(int r=0; r<size; r++)
  {
    //mtx[r] = aligned_alloc(16, size*sizeof(int*));
    mtx[r] = malloc(size*sizeof(int));
  }
  printf("After malloc row\n");

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
  return mtx;
}

int ** initNextMatrix(int **mtx, int size, int **frommtx) {
  // Initializes matrix mtx with values from frommtx
  for(int r=0; r<size; r++)
  {
    //mtx[r] = aligned_alloc(16, size*sizeof(int*));
    mtx[r] = malloc(size*sizeof(int));
  }
  copyMatrixValues(frommtx, mtx, size);
  //copyDMatrix(frommtx, mtx, size);
  return mtx;
}

void copyMatrixValues(int **frommtx, int **tomtx, int size)
{
    // Copies the values of frommtx to tomtx
    for (int r=0; r<size; r++) {
        for (int c=0; c<size; c++) {
            tomtx[r][c] = frommtx[r][c];
        }
    }
}

/*int ** copyDMatrix(int **frommtx, int **tomtx, int size) {
    // Copies the values of frommtx to tomtx
    int b = sizeof(int) * size * size;
    frommtx = memcpy (tomtx, frommtx, b);
    return frommtx;
}*/

void prettyPrintMatrix(int **mtx, int size)
{
    for(int i=0; i<size; ++i) {
        for(int j=0; j<size; ++j)
        {
            printf("%d  ", mtx[i][j]);
        }
        printf("\n\n");
    }
    printf("\n");
}

void usage()
{
  //printf("Usage: %d N\n", program);
  //printf("  N: size of matrix\n");
  printf("please supply argument for size of matrix, greater than %d\n", B_TILE_SIZE);
  exit(1);
}
