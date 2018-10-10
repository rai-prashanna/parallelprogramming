// gcc ex3tiled.c -o ex3tiled
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int id;
    int offset_i;
    int offset_j;
} tile;

#define INIT_NULL_TILE(X) tile X = {.id = 0, .offset_i = -1, .offset_j = -1};


const int MAX_RAND = 101; // If 101, we use infinity
const int INFINITY = 9999;
const int B_TILE_SIZE = 4;
int * pInt = NULL;
tile NULL_TILE = {0 , -1, -1};

void initMatrix(int **mtx, int size);
//void displayMatrix(const int mtx[MATRIX_SIZE][MATRIX_SIZE]);
void prettyPrintMatrix(int **mtx, int size);
void floydWarshallTiled(int **mtx, int size);
void floydWarshallCore(int **mtx, int size, tile t);
void floydWarshallStandard(int **mtx, int size);
tile getEastTile(tile cr, int size);
tile getWestTile(tile cr, int size);
tile getNorthTile(tile cr, int size);
tile getSouthTile(tile cr, int size);
void printTile(tile cr);
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

    printf("Running with size matrix = %d\n", n_mtxsize);
    printf("Tiled version with B tile size = %d\n\n", B_TILE_SIZE);

    int **D_matrix;
    //int D_matrix[MATRIX_SIZE][MATRIX_SIZE];

    initMatrix(D_matrix, n_mtxsize);

    prettyPrintMatrix(D_matrix, n_mtxsize);

    floydWarshallTiled(D_matrix, n_mtxsize);

    prettyPrintMatrix(D_matrix, n_mtxsize);
    
    free(D_matrix);

    return 0;
}

void floydWarshallTiled(int **mtx, int size) {
    // In k=0, we set the edge costs
    // n step B: skip every B-tile size
    for (int k=1; k<size; k=k+B_TILE_SIZE) {
        printf("Now on k = %d\n", k);
        // CR tile
        tile cr_tile = {
            .id = 1,
            .offset_i = 2,
            .offset_j = 3
        };
        floydWarshallCore(mtx, B_TILE_SIZE, cr_tile);

        // E, W, N, S tiles
        tile t = getEastTile(cr_tile, size);
        floydWarshallCore(mtx, B_TILE_SIZE, t);

        t = getWestTile(cr_tile, size);
        floydWarshallCore(mtx, B_TILE_SIZE, t);

        t = getNorthTile(cr_tile, size);
        floydWarshallCore(mtx, B_TILE_SIZE, t);

        t = getSouthTile(cr_tile, size);
        floydWarshallCore(mtx, B_TILE_SIZE, t);

        // TODO: NE, NW, SE, SW tiles

    }
}

void floydWarshallCore(int **mtx, int size, tile t) {
    // This needs to operate on just the tile block of the matrix
    // size is now the block tile size, not the matrix size
    for (int k=1; k<size; k++) {
        printf("  floydWarshallCore now k = %d\n", k);
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
        prettyPrintMatrix(mtx, size);
    }
}

tile getEastTile(tile cr, int size) {
    // Get the east tile if there is one
    if(cr.offset_j+B_TILE_SIZE < size) {
        tile t = {
            //id=
            .offset_i = cr.offset_i,
            .offset_j = cr.offset_j+B_TILE_SIZE
        };
        return t;
    }

    return NULL_TILE;
}
tile getWestTile(tile cr, int size) {
    // Get the west tile if there is one
    if(cr.offset_j > B_TILE_SIZE) {
        tile t = {
            //id=
            .offset_i = cr.offset_i,
            .offset_j = cr.offset_j-B_TILE_SIZE
        };
        return t;
    }

    return NULL_TILE;
}
tile getNorthTile(tile cr, int size) {
    // Get the north tile if there is one
    if(cr.offset_i > B_TILE_SIZE) {
        tile t = {
            //id=
            .offset_i = cr.offset_i - B_TILE_SIZE,
            .offset_j = cr.offset_j
        };
        return t;
    }

    return NULL_TILE;
}
tile getSouthTile(tile cr, int size) {
    // Get the south tile if there is one
    if(cr.offset_i+B_TILE_SIZE<size) {
        tile t = {
            //id=
            .offset_i = cr.offset_i + B_TILE_SIZE,
            .offset_j = cr.offset_j
        };
        return t;
    }

    return NULL_TILE;
}

void printTile(tile cr) {
    printf("Tile id %d, offset_i %d, offset_j %d\n", cr.id, cr.offset_i, cr.offset_j);
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
  //printf("Usage: %d N\n", program);
  //printf("  N: size of matrix\n");
  printf("please supply arugment for size of matrix");
  exit(1);
}
