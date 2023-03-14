// mpicc file.c -o file
// mpirun -np 4 ./file
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
   int rank, size;
   int msg = 31337;
   int buf;

   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   if (rank == 0) {
      printf("Manager: esperando el mensaje...\n");
      for (int i = 1; i < size; i++) {
      MPI_Recv(&buf, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status); //MPI_ANY_SOURCE
      printf("Mensaje: %d\n", buf);
      }
   } else {
      printf("Worker: mandando mensaje...\n");
      MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);// EL primer 0 es a quién se lo mando, el segundo es el tag
   }
   
   MPI_Finalize();
return 0;
}