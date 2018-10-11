// A tiled version of FW
// gcc -Wall -fopenmp ex3-tiled.c -o ex3-tiled
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <omp.h>

typedef struct {
    int id;
    int offset_i;
    int offset_j;
} tile;


#define CACHE_LINE_SIZE = 64;
const int intsizebytes = 4;
// Set tile size to a 4x4 matrix (64/4) = 16 integer values. So 4x4
const int B_TILE_SIZE = 2;
//const int B_TILE_SIZE = 16;

const int MAX_RAND = 101; // If 101, we use infinity
const int INFINITY_VAL = 9999;
int * pInt = NULL;

int ** initMatrix(int **mtx, int size);
void prettyPrintMatrix(int **mtx, int size);
void floydWarshallTiledParallel(int **mtx, int size, int **next_mtx);
void floydWarshallTiledSerial(int **mtx, int size);
void floydWarshallTileCoreParallel(int **mtx, int size, int k, int **next_mtx);
void floydWarshallTileCore(int **mtx, int size, int k);
int ** initNextMatrix(int **mtx, int size, int **frommtx);
void copyMatrixValues(int **frommtx, int **tomtx, int size);
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

    floydWarshallTiledParallel(D_matrix, n_mtxsize, D_matrix_nextk);
    //floydWarshallTiledSerial(D_matrix, n_mtxsize);
    printf("floydWarshallTiled done\n");

    prettyPrintMatrix(D_matrix, n_mtxsize);

    printf("Freeing resources\n");
    free(D_matrix);
    free(D_matrix_nextk);
    printf("Done\n");

    return 0;
}

void floydWarshallTiledSerial(int **mtx, int size) {
    // In k=0, we set the edge costs
    // n step B: skip every B-tile size
    int num_tiles_per_dim = size / B_TILE_SIZE;

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
        int t_max_col_j = t_base_col_j+size-1;
        
        int t_base_row_i = floor(k % B_TILE_SIZE)-1;  // todo -1 should be 0
        int t_max_row_i = t_base_row_i + (num_tiles_per_dim-1)*size;
        
        for (int t=t_base_col_j; t<t_max_col_j; t=t+B_TILE_SIZE) {
            // These iterates over E, W tiles
            // Skip cr tile
            if (t==k) continue;
            printf("Would iterate over E, W tiles with k %d\n", t);
            floydWarshallTileCore(mtx, size, k);
        }

        for (int t=t_base_row_i; t<t_max_row_i; t=t+size) {
            // These iterates over N, S tiles
            // Skip cr tile
            if (t==k) continue;
            printf("Would iterate over N, S tiles with k %d\n", t);
        }

        // TODO: NE, NW, SE, SW tiles
    }
}

void floydWarshallTiledParallel(int **mtx, int size, int **next_mtx) {
    // In k=0, we set the edge costs
    // n step B: skip every B-tile size
    printf("Using the parallel version of tiled");
    int num_tiles_per_dim = size / B_TILE_SIZE;

    // Do we want to omp collapse?
    // over k. reads: mtx, size, B_TILE_SIZE, 
    #pragma omp parallel for firstprivate(size) shared(mtx, next_mtx) num_threads(4)
    for (int k=0; k<size; k=k+B_TILE_SIZE) {
        printf("Now on k = %d\n", k);
        
        int ID = omp_get_thread_num();
        printf("I am thread %d", ID);

        floydWarshallTileCoreParallel(mtx, size, k, next_mtx);

        // TODO: E, W, N, S tiles
        // Is it better to collect tiles in array or like below? For parallelize?        
        int t_base_col_j = floor(k / size)*size;
        int t_max_col_j = t_base_col_j+size-1;
        
        int t_base_row_i = floor(k % B_TILE_SIZE)-1;  // todo -1 should be 0
        int t_max_row_i = t_base_row_i + (num_tiles_per_dim-1)*size;
        
        for (int t=t_base_col_j; t<t_max_col_j; t=t+B_TILE_SIZE) {
            // These iterates over E, W tiles
            // Skip cr tile
            if (t==k) continue;
            printf("Would iterate over E, W tiles with k %d\n", t);
            floydWarshallTileCoreParallel(mtx, size, k, next_mtx);
        }

        for (int t=t_base_row_i; t<t_max_row_i; t=t+size) {
            // These iterates over N, S tiles
            // Skip cr tile
            if (t==k) continue;
            printf("Would iterate over N, S tiles with k %d\n", t);
            floydWarshallTileCoreParallel(mtx, size, k, next_mtx);
        }

        // TODO: NE, NW, SE, SW tiles

        // All threads must have completed the k iteration before we copy values over to the original mtx
        #pragma omp barrier
        {
            printf("Copying the values from next_mtx into mtx");
            copyMatrixValues(next_mtx, mtx, size);
        }
    }
}

void floydWarshallTileCoreParallel(int **mtx, int size, int k, int **next_mtx) {
    // Reads from mtx, writes to next_mtx
    printf("  floydWarshallCore now k = %d\n", k);
    for(int j=0; j<size; ++j) {
        for(int i=0; i<size; ++i) {
            // Set mtx[i][j] for k+1 to the minimum
            // can skip where i=j
            if (i==j)  continue;

            int tmpcost = mtx[i][k] + mtx[k][j];
            if (tmpcost < mtx[i][j]) {
                //printf("Changing cost from %d to %d\n", mtx[i][j], tmpcost);
                #pragma omp critical
                {
                    next_mtx[i][j] = tmpcost;
                }
            }
            else {
                //printf("Keeping current cost at %d\n", mtx[i][j]);
            }
        }
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
                //printf("Changing cost from %d to %d\n", mtx[i][j], tmpcost);
                mtx[i][j] = tmpcost;
            }
            else {
                //printf("Keeping current cost at %d\n", mtx[i][j]);
            }
        }
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
                w = INFINITY_VAL;
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
