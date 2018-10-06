#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
    int i;

    #pragma omp parallel num_threads(8)
    {
        #pragma omp for schedule(static,1)
        for (i = 0; i < 8; i++)
            printf("[1] iter %0d, tid %0d\n", i, omp_get_thread_num());

        #pragma omp for schedule(static,1)
        for (i = 0; i < 8; i++)
            printf("[2] iter %0d, tid %0d\n", i, omp_get_thread_num());
    }

    return 0;
}
