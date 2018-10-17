#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main(int argc, char *argv[])
{
  int rank, size, nr;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
      // Send to process 1
      nr = 99;
      MPI_Send(&nr, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  }
  else if (rank == 1) {
      // Receive from process 0
      MPI_Recv(&nr, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("Process received nr %d \n", nr);
  }
  else {
    printf("Another process, not 0 or 1, with rank %d \n", rank);
  }

  MPI_Finalize();

}