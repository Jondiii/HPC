// mpicc file.c -o file
// mpirun -np 4 ./file
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
   int rank, size;
   int msg = 0;
   int buf;

   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   if (rank == 0) {
      printf("Manager: esperando el mensaje...\n");
      while (1){
         MPI_Send(&msg, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
         //printf("Manager: mandando mensaje...  \n");
         MPI_Recv(&msg, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, &status); //MPI_ANY_SOURCE
         msg++;
         printf("Mensaje: %d\n", msg);
      }
   } else {
      while (1) {
         //printf("Worker %d: recibiendo mensaje...\n", rank);
         MPI_Recv(&msg, 1, MPI_INT,  rank - 1, 0, MPI_COMM_WORLD, &status); //MPI_ANY_SOURCE
         msg++;
         //printf("Worker %d: mandando mensaje...\n", rank);
         MPI_Send(&msg, 1, MPI_INT, (rank +1) % size, 0, MPI_COMM_WORLD); // rank + 1 % size

      }
   }
   
   MPI_Finalize();
return 0;
}