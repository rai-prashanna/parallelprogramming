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


const int MATRIX_SIZE = 4;
const int MAX_RAND = 101; // If 101, we use infinity
const int INFINITY = 9999;
const int B_TILE_SIZE = 2; //MATRIX_SIZE;
int * pInt = NULL;
tile NULL_TILE = {0 , -1, -1};

void initMatrix(int mtx[MATRIX_SIZE][MATRIX_SIZE]);
void displayMatrix(const int mtx[MATRIX_SIZE][MATRIX_SIZE]);
void prettyPrintMatrix(const int mtx[MATRIX_SIZE][MATRIX_SIZE]);
void floydWarshallStandard(int mtx[MATRIX_SIZE][MATRIX_SIZE]);
void floydWarshall(int mtx[MATRIX_SIZE][MATRIX_SIZE], int num_tiles);
tile getEastTile(tile cr);
tile getWestTile(tile cr);
tile getNorthTile(tile cr);
tile getSouthTile(tile cr);
void printTile(tile cr);

int main()
{
    int D_matrix[MATRIX_SIZE][MATRIX_SIZE];

    initMatrix(D_matrix);

    prettyPrintMatrix(D_matrix);

    floydWarshallStandard(D_matrix);

    prettyPrintMatrix(D_matrix);

    /*tile cr_tile = {
        .id = 1,
        .offset_i = 2,
        .offset_j = 3
    };

    tile easttile = getEastTile(cr_tile);
    tile westtile = getWestTile(cr_tile);
    tile northtile = getNorthTile(cr_tile);
    tile southtile = getSouthTile(cr_tile);

    printTile(easttile);
    printTile(westtile);
    printTile(northtile);
    printTile(southtile);*/
    
    return 0;
}

void floydWarshallStandard(int mtx[MATRIX_SIZE][MATRIX_SIZE]) {
    // In k=0, we set the edge costs
    for (int k=1; k<MATRIX_SIZE; k++) {
        printf("Now on k = %d\n", k);
        for(int j=1; j<MATRIX_SIZE; ++j) {
            for(int i=1; i<MATRIX_SIZE; ++i) { 
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
        prettyPrintMatrix(mtx);
    }
}

void floydWarshall(int mtx[MATRIX_SIZE][MATRIX_SIZE], int num_tiles) {
    // k = current block iteration
    for (int k=0; k<num_tiles; k++) {
        for(int j=0; j<B_TILE_SIZE; ++j) {
            for(int i=0; i<B_TILE_SIZE; ++i) { 
                // Set mtx[i][j] for k+1 to the minimum
                ///if (mtx[i][j] =
                ///mtx[i][j] =
            }
        }
    }
}

tile getEastTile(tile cr) {
    // Get the east tile if there is one
    if(cr.offset_j+B_TILE_SIZE < MATRIX_SIZE) {
        tile t = {
            //id=
            .offset_i = cr.offset_i,
            .offset_j = cr.offset_j+B_TILE_SIZE
        };
        return t;
    }

    return NULL_TILE;
}
tile getWestTile(tile cr) {
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
tile getNorthTile(tile cr) {
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
tile getSouthTile(tile cr) {
    // Get the south tile if there is one
    if(cr.offset_i+B_TILE_SIZE<MATRIX_SIZE) {
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

void initMatrix(int mtx[MATRIX_SIZE][MATRIX_SIZE]) {
    int seed = time(NULL);
    srand(seed);

     for(int i=0; i<MATRIX_SIZE; ++i) {
        for(int j=0; j<MATRIX_SIZE; ++j)
        {
            if (i==j) {
                mtx[i][j] = 0;
            }
            else {
                // Sometimes, use inifinity as cost
                int r = rand() % MAX_RAND;

                if (r==MAX_RAND) {
                    r=INFINITY;
                }

                mtx[i][j] = r;
            }
        }
    }
}

void prettyPrintMatrix(const int mtx[MATRIX_SIZE][MATRIX_SIZE]) {
    for(int i=0; i<MATRIX_SIZE; ++i) {
        printf("\n\n");
        for(int j=0; j<MATRIX_SIZE; ++j)
        {
            printf("%d  ", mtx[i][j]);
        }
    }
    printf("\n");
}

void displayMatrix(const int mtx[MATRIX_SIZE][MATRIX_SIZE]) {
    for(int i=0; i<MATRIX_SIZE; ++i) {
        for(int j=0; j<MATRIX_SIZE; ++j)
        {
            printf("Value at cell i %d, j %d is: ",i, j, &mtx[i][j]);
        }
    }
}
