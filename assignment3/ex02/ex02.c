/******************************************************
 ************* Conway's game of life ******************
 ******************************************************

 Usage: ./exec ArraySize TimeSteps

 Compile with -DOUTPUT to print output in output.gif
 (You will need ImageMagick for that - Install with
  sudo apt-get install imagemagick)
 WARNING: Do not print output for large array sizes!
          or multiple time steps!
 ******************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
#define FINALIZE "\
convert -delay 20 out*.pgm output.gif\n\
rm *pgm\n\
"

static int ** allocate_array(int N)
{
    int ** array;
    int i, j;
    array = malloc(N * sizeof(int*));
    for (i = 0; i < N ; i++)
        array[i] = malloc(N * sizeof(int));
    for (i = 0; i < N ; i++)
        for (j = 0; j < N ; j++)
            array[i][j] = 0;
    return array;
}

static void free_array(int ** array, int N)
{
    int i;
    for (i = 0 ; i < N ; i++)
        free(array[i]);
    free(array);
}

static void init_random(int ** array1, int ** array2, int N)
{
    int i, pos;

    for (i = 0 ; i < (N * N)/10 ; i++)
    {
        pos = rand() % ((N-2)*(N-2));
        array1[pos%(N-2)+1][pos/(N-2)+1] = 1;
        array2[pos%(N-2)+1][pos/(N-2)+1] = 1;

    }
}

#ifdef OUTPUT
static void print_to_pgm(int ** array, int N, int t)
{
    int i, j;
    char * s = malloc(30*sizeof(char));
    sprintf(s, "out%d.pgm", t);
    FILE * f = fopen(s, "wb");
    fprintf(f, "P5\n%d %d 1\n", N,N);
    for (i = 0; i < N ; i++)
        for (j = 0; j < N ; j++)
            if (array[i][j] == 1)
                fputc(1, f);
            else
                fputc(0, f);
    fclose(f);
    free(s);
}
#endif

int main (int argc, char * argv[])
{
    int N;	 			//array dimensions
    int T; 				//time steps
    int ** current, ** previous; 	//arrays - one for current timestep, one for previous timestep
    int ** swap;			//array pointer
    int t, i, j, nbrs;		//helper variables

    double time;			//variables for timing
    struct timeval ts,tf;

    /*Read input arguments*/
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./exec ArraySize TimeSteps\n");
        exit(-1);
    }
    else
    {
        N = atoi(argv[1]);
        T = atoi(argv[2]);
    }

    /*Allocate and initialize matrices*/
    current = allocate_array(N);			//allocate array for current time step
    previous = allocate_array(N); 			//allocate array for previous time step

    init_random(previous, current, N);	//initialize previous array with pattern

#ifdef OUTPUT
    print_to_pgm(previous, N, 0);
#endif

    /*Game of Life*/

    gettimeofday(&ts,NULL);
    int  id, col_chunk_size, start_index, end_index;
    int numofthreads=2;
    for (t = 0 ; t < T ; t++)
    {
        #pragma omp parallel private(nbrs,id) shared(N,previous,current) num_threads(16)
        id = omp_get_thread_num();
        int N_threads = omp_get_num_threads();
        /* distribute cols to different threads */
        col_chunk_size = N-1 / N_threads;
        if(id==0)
        {
            start_index=1;
        }
        else
        {
            start_index = id * col_chunk_size;
        }
        end_index = (id+1) * col_chunk_size;
        if (id == N_threads - 1)
        {
            end_index = N-1;
        }

        omp_set_dynamic(0);
        omp_set_num_threads(N_threads);

        for (i = start_index; i < end_index ; i++)
            for (j = start_index ; j < end_index ; j++)
            {
                nbrs = previous[i+1][j+1] + previous[i+1][j] + previous[i+1][j-1] \
                       + previous[i][j-1] + previous[i][j+1] \
                       + previous[i-1][j-1] + previous[i-1][j] + previous[i-1][j+1];
                #pragma omp critical
                {
                    if (nbrs == 3 || ( previous[i][j]+nbrs == 3))
                        current[i][j] = 1;
                    else
                        current[i][j] = 0;
                }
            }


#ifdef OUTPUT
        print_to_pgm(current, N, t+1);
#endif
        //Swap current array with previous array
        swap = current;
        current = previous;
        previous = swap;

    }
    gettimeofday(&tf,NULL);
    time = (tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;

    free_array(current, N);
    free_array(previous, N);
    printf("GameOfLife: Size %d Steps %d Time %lf\n", N, T, time);
#ifdef OUTPUT
    system(FINALIZE);
#endif
    return 0;
}
