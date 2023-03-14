// mpicc file.c -o file
// mpirun -np 4 ./file
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
   int rank, size;
   int msg = 0;
   int buf;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   if (rank == 0) {
      msg = 10000;
   }

   MPI_Bcast(&msg, 1, MPI_INT, 0, MPI_COMM_WORLD); // NO hace falta un receive. EL 0 indica el origen del mensaje
   //MPI_Recv(&msg, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, &status); //MPI_ANY_SOURCE
   printf(" Soy el proceso %d y estás viendo disney channel %d\n", rank, msg);
   MPI_Finalize();
return 0;
}