/*
2 * A simple microbenchmark to observe the effects of caches
3 * when walking through arrays of different sizes , either in
4 * order or randomly .
5 *
6 * Written by Pontus Ekberg <pontus . ekberg@it .uu.se >
7 * Last updated : 2018 -09 -26
8 */
# include <stdlib.h>
# include <stdio.h>
# include <time.h>

# define MIN_ELEMENTS (1 << 8) /* 2ˆ8 */
# define MAX_ELEMENTS (1 << 21) /* 2ˆ21 */
# define ITERATIONS_PER_TEST 100000000

/* Return a uniformly random int in the range [0 , n -1]. */

int rand_int ( int n)
{
    int limit = RAND_MAX - RAND_MAX % n;
    int rnd ;

    do
    {
        rnd = rand () ;

    }
    while ( rnd >= limit );
    return rnd % n;

}

/* Randomly shuffle the elements of an array . */
void shuffle ( int * array, size_t n)
{
    int i, j, tmp ;

    for (i = n - 1; i > 0; i--)
    {
        j = rand_int (i + 1) ;
        tmp = array [ j ];
        array [j] = array [i ];
        array [i] = tmp ;

    }

}

/*
42 * Create an array of <size > elements 0, 1, ... , size -1.
43 * The elements are randomly arranged so that they create a
44 * big cycle through all the elements if one follows them
45 * by picking the next element index as the value of the
46 * current element .
47 */
int * make_random_array ( size_t size )
{
    int * tmp_array = malloc ( size * sizeof ( int ));
    int * array = malloc ( size * sizeof ( int ));
    for (int i = 0; i < size ; i ++)
    {
        tmp_array [ i] = i;

    }
    shuffle ( tmp_array, size );

    int j;
    for (int i = 0; i < size ; i ++)
    {
        j = tmp_array [ i ];
        array [j] = tmp_array [( i +1) % size ];

    }

    free ( tmp_array );
    return array ;

}

int * make_order_array ( size_t size )
{
    int * array = malloc ( size * sizeof ( int ));

    for (int i = 0; i < size ; i ++)
    {
        array [i] = (i + 1) == size ? 0 : i + 1;

    }

    return array ;
}
/*
77 * Chase data in an array by selecting the index of the next element
78 * as the value of the current element .
79 */
void loop_scan_array (int * array, size_t size, int iterations )
{
    int index = 0;
    for (int i = 0; i < iterations ; i ++)
    {
        index = array [ index ];

    }
    return ;

}

int main ()
{

    /* Time chasing of data in random matrices of different sizes . */
    for ( size_t size = MIN_ELEMENTS ; size <= MAX_ELEMENTS ; size *= 2)
    {
        int * array = make_order_array ( size ) ;

        clock_t start = clock () ;
        loop_scan_array ( array, size, ITERATIONS_PER_TEST );
        clock_t end = clock () ;

        free ( array );

        printf (" %5.2f seconds spent on chasing data in array of %4 zu KB\n",
                ( double ) ( end - start ) / CLOCKS_PER_SEC,
                size * sizeof ( int) / 1024) ;

    }

    return 0;

}
